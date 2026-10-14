/*
 * at24c256.c
 *
 *  Created on: Aug 13, 2026
 *      Author: TRUONGGIANG
 */
#include "at24c256.h"

#define PAGE_SIZE 64U
#define ADDRESS_7BIT (0X50 << 1)

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef status;
HAL_StatusTypeDef status_1;
HAL_StatusTypeDef status_2;
uint8_t page_offset = 0;
uint8_t page_space = 0;
uint16_t write_size = 0;

HAL_StatusTypeDef Write_AT24(uint16_t memory_address, uint8_t *data_buffer, uint16_t length)
{
	while(length > 0)
	{
		page_offset = memory_address % PAGE_SIZE;
		page_space = PAGE_SIZE - page_offset;
		write_size = page_space > length ? length : page_space;
		status = HAL_I2C_Mem_Write(&hi2c1, ADDRESS_7BIT, memory_address, I2C_MEMADD_SIZE_16BIT, data_buffer, write_size, 100);
		if(status == HAL_OK)
		{
			HAL_Delay(10);
			status_1 = HAL_I2C_IsDeviceReady(&hi2c1, ADDRESS_7BIT, 3, 100);
			if(status_1 == HAL_OK)
			{
				data_buffer += write_size;
				memory_address += write_size;
				length -= write_size;
				if (length == 0) return HAL_OK;
			}
			else return HAL_ERROR;
		}
		else return HAL_ERROR;
	}
	return HAL_ERROR;
}

HAL_StatusTypeDef Read_AT24(uint16_t memory_address, uint8_t *data_buffer, uint16_t length)
{
	status_2 = HAL_I2C_Mem_Read(&hi2c1, ADDRESS_7BIT, memory_address, I2C_MEMADD_SIZE_16BIT, data_buffer, length, 100);
	if(status_2 == HAL_OK) return HAL_OK;
	else return HAL_ERROR;
}


