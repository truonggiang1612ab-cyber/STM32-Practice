/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "at24c256.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	uint16_t index_record;
	uint16_t voltage;
	uint8_t duty_cycle;
}conFig_t;
conFig_t System;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIMEOUT 100U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
GPIO_PinState previodstate = GPIO_PIN_SET;
uint32_t button_tick = 0;
uint8_t detect_change = 0;
uint32_t raw = 0;
uint16_t milivolt = 0;
uint8_t milivolt_integer = 0;
uint16_t milivolt_decimal = 0;
uint8_t duty_cal = 0;
uint16_t ccr_pwm = 0;
char command_uart[150];
uint8_t overflow = 0;
uint16_t index_record = 0;
uint16_t mem_add_write = 0;
uint16_t mem_add_read = 0;
int leng = 0;
uint8_t next_record = 0;
uint16_t counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t button(void);
uint8_t read_adc(void);
uint8_t duty_and_ccr_pwm(void);
void receive_command_uart(void);
void receive_buffer_info(void);
void receive_buffer_invalid(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t button(void)
{
	GPIO_PinState currentstate;
	currentstate = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	if(detect_change == 0 && currentstate != previodstate)
	{
		detect_change = 1;
		button_tick = HAL_GetTick();
	}
	else if(detect_change == 1)
	{
		if(HAL_GetTick() - button_tick < TIMEOUT) return 0;
		currentstate = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		if(currentstate != previodstate)
		{
			previodstate = currentstate;
			if(currentstate == GPIO_PIN_RESET)
			{
				detect_change = 0;
				return 1;
			}
		}
		detect_change = 0;
	}
	return 0;
}
uint8_t read_adc(void)
{
	if(HAL_ADC_Start(&hadc1) == HAL_OK)
	{
		if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
		{
			raw = HAL_ADC_GetValue(&hadc1);
			milivolt = (raw * 3300) / 4095;
			milivolt_integer = milivolt / 1000;
			milivolt_decimal = milivolt % 1000;
			return 1;
		}
	}
	return 0;
}
uint8_t duty_and_ccr_pwm(void)
{
	if(read_adc() == 1)
	{
		duty_cal = (milivolt * 100) / 3300;
		ccr_pwm = (duty_cal * 1000) / 100;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, ccr_pwm);
		return 1;
	}
	else return 0;
}
void receive_command_uart(void)
{
	char temp_byte;
	static uint8_t index_byte = 0;
	if(HAL_UART_Receive(&huart1, (uint8_t *) &temp_byte, 1, 100) == HAL_OK)
	{
		if(overflow == 0)
		{
			if(index_byte <= 148 && temp_byte != '\n')
			{
				command_uart[index_byte] = temp_byte;
				index_byte++;
			}
			else if(index_byte > 148 && temp_byte != '\n')
			{
				overflow  = 1;
				leng = snprintf(command_uart, sizeof(command_uart), "ĐANG TRÀN BỘ ĐỆM\r\n");
				HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], leng, 100);
			}
			else if(temp_byte == '\n' && index_byte != 0)
			{
				command_uart[index_byte] = '\0';
				index_byte = 0;
				if(strcmp(command_uart, "info") == 0)
				{
					counter = 0;
					mem_add_read = 0;
					receive_buffer_info();
				}
				else
				{
					receive_buffer_invalid();
				}
			}
		}
		else if(overflow == 1)
		{
			if(temp_byte == '\n')
			{
				index_byte = 0;
				overflow = 0;
			}
		}
	}
}
void create_record(void)
{
	if(duty_and_ccr_pwm() == 1)
	{
		System.index_record = index_record+ 1;
		System.voltage = milivolt;
		System.duty_cycle = duty_cal;
		if(mem_add_write + sizeof(System) <= 32768)
		{
			if(Write_AT24(mem_add_write, (uint8_t *)&System, sizeof(System)) == HAL_OK)
			{
				mem_add_write += sizeof(System);
				index_record++;
				next_record = snprintf(command_uart, sizeof(command_uart), "GHI THÀNH CÔNG\r\n");
				next_record += snprintf(command_uart + next_record, sizeof(command_uart) - next_record, "Bản ghi:%d\r\n", index_record);
				next_record += snprintf(command_uart + next_record, sizeof(command_uart) - next_record, "Điện áp:%d.%.3dV\r\n", milivolt_integer, milivolt_decimal);
				next_record += snprintf(command_uart + next_record, sizeof(command_uart) - next_record, "Duty_cycle:%d%%\r\n", duty_cal);
				HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], next_record, 100);
			}
			else
			{
				leng = snprintf(command_uart, sizeof(command_uart), "QUÁ TRÌNH GHI THẤT BẠI\r\n");
				HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], leng, 100);
			}
		}
		else
		{
			leng = snprintf(command_uart, sizeof(command_uart), "BỘ NHỚ ĐÃ ĐẦY\r\n");
			HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], leng, 100);
		}
	}
	else
	{
		leng = snprintf(command_uart, sizeof(command_uart), "CÓ LỖI TỪ PHÍA ADC\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], leng, 100);
	}
}
void button_loop(void)
{
	if(button() == 1)
	{
		create_record();
	}
}
void receive_buffer_info(void)
{
	static uint8_t read_vol_integer = 0;
	static uint16_t read_vol_decimal = 0;
	while(counter < index_record)
	{
		if(Read_AT24(mem_add_read, (uint8_t *) &System, sizeof(System)) == HAL_OK)
		{
			read_vol_integer = System.voltage / 1000;
			read_vol_decimal = System.voltage % 1000;
			next_record = snprintf(command_uart, sizeof(command_uart), "ĐỌC THÀNH CÔNG\r\n");
			next_record += snprintf(command_uart + next_record, sizeof(command_uart) - next_record, "Bản ghi:%d\r\n", System.index_record);
			next_record += snprintf(command_uart + next_record, sizeof(command_uart) - next_record, "Điện áp:%d.%.3dV\r\n", read_vol_integer, read_vol_decimal);
			next_record += snprintf(command_uart + next_record, sizeof(command_uart) - next_record, "Duty_cycle:%d%%\r\n", System.duty_cycle);
			HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], next_record, 100);
			mem_add_read += sizeof(System);
			counter++;
		}
		else
		{
			leng = snprintf(command_uart, sizeof(command_uart), "ĐỌC THẤT BẠI\r\n");
			HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], leng, 100);
			return;
		}
	}
	if(index_record == 0)
	{
		leng = snprintf(command_uart, sizeof(command_uart), "KHÔNG TỒN TẠI BẢN GHI\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], leng, 100);
		return;
	}
}
void receive_buffer_invalid(void)
{
	leng = snprintf(command_uart, sizeof(command_uart), "LỆNH KHÔNG HỢP LỆ\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t *) &command_uart[0], leng, 100);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1)
  {
	  button_loop();
	  receive_command_uart();
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */
  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */
  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */
  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */
  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */
  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */
  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */
  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */
  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */
  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

