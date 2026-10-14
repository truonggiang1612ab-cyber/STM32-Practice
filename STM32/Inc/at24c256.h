/*
 * at24c256.h
 *
 *  Created on: Aug 13, 2026
 *      Author: TRUONGGIANG
 */

#ifndef INC_AT24C256_H_
#define INC_AT24C256_H_

#include "stm32f1xx_hal.h"

HAL_StatusTypeDef Write_AT24(uint16_t memory_address, uint8_t *data_buffer, uint16_t length);
HAL_StatusTypeDef Read_AT24(uint16_t memory_address, uint8_t *data_buffer, uint16_t length);

#endif /* INC_AT24C256_H_ */
