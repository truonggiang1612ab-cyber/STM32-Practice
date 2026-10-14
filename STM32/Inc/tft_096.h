/*
 * tft_096.h
 *
 *  Created on: Aug 29, 2026
 *      Author: TRUONGGIANG
 */

#ifndef INC_TFT_096_H_
#define INC_TFT_096_H_

#include "stdint.h"
#include "stm32f1xx_hal.h"


typedef struct
{
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *CS_port;
	uint16_t CS_pin;
	GPIO_TypeDef *DC_port;
	uint16_t DC_pin;
	GPIO_TypeDef *BL_port;
	uint16_t BL_pin;

}TFT_HandleTypeDef;

uint8_t Init_TFT(TFT_HandleTypeDef *ConFig_TFT);
uint8_t Fill_Rectangle(TFT_HandleTypeDef *ConFig_TFT, uint8_t col_start, uint8_t row_start, uint8_t col_stop, uint8_t row_stop, uint8_t red, uint8_t green, uint8_t blue);
uint8_t Fill_Screen(TFT_HandleTypeDef *ConFig_TFT, uint8_t red, uint8_t green, uint8_t blue);
uint8_t Print_String(TFT_HandleTypeDef *ConFig_TFT, const char *string, uint8_t col_coordinate, uint8_t row_coordinate, uint8_t red, uint8_t green, uint8_t blue);

#endif /* INC_TFT_096_H_ */















