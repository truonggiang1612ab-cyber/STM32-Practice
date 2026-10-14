/*
 * tft_096.c
 *
 *  Created on: Aug 29, 2026
 *      Author: TRUONGGIANG
 */

#include "tft_096.h"

#define SWRESET 0x01
#define SLPOUT 0x11
#define FRMCTR1 0xB1
#define FRMCTR2 0xB2
#define FRMCTR3 0xB3
#define INVCTR 0xB4
#define DATA_BYTE_1 0x01
#define DATA_BYTE_2 0x2C
#define DATA_BYTE_3 0x2D
#define DATA_BYTE_4 0x07
#define PWCTR1 0xC0
#define DATA_BYTE_5 0xA2
#define DATA_BYTE_6 0x02
#define DATA_BYTE_7 0x84
#define PWCTR2 0xC1
#define DATA_BYTE_8 0xC5
#define PWCTR3 0xC2
#define DATA_BYTE_9 0x0A
#define DATA_BYTE_10 0x00
#define PWCTR4 0xC3
#define DATA_BYTE_11 0x8A
#define DATA_BYTE_12 0x2A
#define PWCTR5 0xC4
#define DATA_BYTE_13 0xEE
#define VMCTR1 0xC5
#define DATA_BYTE_14 0x0E
#define INVON 0x21
#define MADCTL 0x36
#define DATA_BYTE_15 0xA8
#define COLMOD 0x3A
#define DATA_BYTE_16 0x55
#define GMCTRP1 0xE0
#define DATA_BYTE_17 0x1C
#define DATA_BYTE_18 0x12
#define DATA_BYTE_19 0x37
#define DATA_BYTE_20 0x32
#define DATA_BYTE_21 0x29
#define DATA_BYTE_22 0x25
#define DATA_BYTE_23 0x2B
#define DATA_BYTE_24 0x39
#define DATA_BYTE_25 0x03
#define DATA_BYTE_26 0x10
#define GMCTRN1 0xE1
#define DATA_BYTE_27 0x1D
#define DATA_BYTE_28 0x06
#define DATA_BYTE_29 0x2E
#define DATA_BYTE_30 0x3F
#define NORON 0x13
#define DISPON 0x29
#define CASET 0x2A
#define RASET 0x2B
#define RAMWR 0x2C

static uint8_t Send_Command_Byte(TFT_HandleTypeDef *ConFig_TFT, uint8_t byte_command)
{
	HAL_GPIO_WritePin(ConFig_TFT->CS_port, ConFig_TFT->CS_pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ConFig_TFT->DC_port, ConFig_TFT->DC_pin,GPIO_PIN_RESET);
	if (HAL_SPI_Transmit(ConFig_TFT->hspi, &byte_command, 1, 50) != HAL_OK)
	{
		HAL_GPIO_WritePin(ConFig_TFT->CS_port, ConFig_TFT->CS_pin,GPIO_PIN_SET);
		return 1;
	}
	HAL_GPIO_WritePin(ConFig_TFT->CS_port, ConFig_TFT->CS_pin,GPIO_PIN_SET);
	return 0;
}

static uint8_t Send_Data_Byte(TFT_HandleTypeDef *ConFig_TFT, uint8_t byte_data)
{
	HAL_GPIO_WritePin(ConFig_TFT->CS_port, ConFig_TFT->CS_pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ConFig_TFT->DC_port, ConFig_TFT->DC_pin,GPIO_PIN_SET);
	if (HAL_SPI_Transmit(ConFig_TFT->hspi, &byte_data, 1, 50) != HAL_OK)
	{
		HAL_GPIO_WritePin(ConFig_TFT->CS_port, ConFig_TFT->CS_pin,GPIO_PIN_SET);
		return 1;
	}
	HAL_GPIO_WritePin(ConFig_TFT->CS_port, ConFig_TFT->CS_pin,GPIO_PIN_SET);
	return 0;
}

uint8_t Init_TFT(TFT_HandleTypeDef *ConFig_TFT)
{
	if(Send_Command_Byte(ConFig_TFT, SWRESET) == 0) return 0;
	HAL_Delay(150);
	if(Send_Command_Byte(ConFig_TFT, SLPOUT) == 0) return 0;
	HAL_Delay(150);
	if(Send_Command_Byte(ConFig_TFT, FRMCTR1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_2) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_3) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, FRMCTR2) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_2) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_3) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, FRMCTR3) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_2) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_3) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_2) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_3) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, INVCTR) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_4) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, PWCTR1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_5) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_6) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_7) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, PWCTR2) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_8) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, PWCTR3) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_9) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_10) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, PWCTR4) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_11) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_12) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, PWCTR5) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_11) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_13) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, VMCTR1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_14) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, INVON) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, MADCTL) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_15) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, COLMOD) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_16) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, GMCTRP1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_6) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_17) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_4) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_18) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_19) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_20) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_21) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_3) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_21) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_22) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_23) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_24) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_10) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_25) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_26) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, GMCTRN1) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_25) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_27) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_4) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_28) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_29) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_2) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_21) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_3) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_29) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_29) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_19) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_30) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_10) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_10) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_6) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, DATA_BYTE_26) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, NORON) == 0) return 0;
	HAL_Delay(20);
	if(Send_Command_Byte(ConFig_TFT, DISPON) == 0) return 0;
	HAL_GPIO_WritePin(ConFig_TFT->BL_port, ConFig_TFT->BL_pin, GPIO_PIN_SET);
	return 1;
}

static uint8_t Set_Window(TFT_HandleTypeDef *ConFig_TFT, uint16_t col_start, uint16_t row_start, uint16_t col_stop, uint16_t row_stop)
{
	if(col_start > 159 || col_stop > 159 || row_start > 79 || row_stop > 79 || col_start > col_stop || row_start > row_stop) return 0;
	uint16_t col_start_offset = col_start + 1;
	uint16_t col_stop_offset = col_stop + 1;
	uint16_t row_start_offset = row_start + 26;
	uint16_t row_stop_offset = row_stop + 26;
	uint8_t high_byte_col_start = col_start_offset >> 8;
	uint8_t low_byte_col_start = col_start_offset;
	uint8_t high_byte_col_stop = col_stop_offset >> 8;
	uint8_t low_byte_col_stop = col_stop_offset;
	uint8_t high_byte_row_start = row_start_offset >> 8;
	uint8_t low_byte_row_start = row_start_offset;
	uint8_t high_byte_row_stop = row_stop_offset >> 8;
	uint8_t low_byte_row_stop = row_stop_offset;
	if(Send_Command_Byte(ConFig_TFT, CASET) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, high_byte_col_start) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, low_byte_col_start) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, high_byte_col_stop) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, low_byte_col_stop) == 0) return 0;
	if(Send_Command_Byte(ConFig_TFT, RASET) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, high_byte_row_start) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, low_byte_row_start) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, high_byte_row_stop) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT, low_byte_row_stop) == 0) return 0;
	return 1;
}

static uint16_t Packet_Color(uint8_t red_color, uint8_t green_color, uint8_t blue_color)
{
	uint8_t five_bit_red = red_color >> 3;
	uint8_t six_bit_green = green_color >> 2;
	uint8_t five_bit_blue = blue_color >> 3;
	uint16_t byte_concatenation = five_bit_red << 11 | six_bit_green << 5 | five_bit_blue;
	return byte_concatenation;
}

static uint8_t Draw_Pixel(TFT_HandleTypeDef *ConFig_TFT,uint8_t col_coordinate, uint8_t row_coordinate, uint8_t red, uint8_t green, uint8_t blue)
{
	if(Set_Window(ConFig_TFT,col_coordinate, row_coordinate, col_coordinate, row_coordinate) == 0) return 0;
	uint16_t receive_byte = Packet_Color(red, green, blue);
	uint8_t high_order_byte = receive_byte >> 8;
	uint8_t low_order_byte = receive_byte;
	if(Send_Command_Byte(ConFig_TFT, RAMWR) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT,high_order_byte) == 0) return 0;
	if(Send_Data_Byte(ConFig_TFT,low_order_byte) == 0) return 0;
	return 1;
}

uint8_t Fill_Rectangle(TFT_HandleTypeDef *ConFig_TFT,uint8_t col_start, uint8_t row_start, uint8_t col_stop, uint8_t row_stop, uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t count = 0;
	if(Set_Window(ConFig_TFT, col_start, row_start, col_stop, row_stop) == 0) return 0;
	uint16_t receive_byte = Packet_Color(red, green, blue);
	uint8_t high_order_byte = receive_byte >> 8;
	uint8_t low_order_byte = receive_byte;
	if(Send_Command_Byte(ConFig_TFT, RAMWR) == 0) return 0;
	uint8_t offset_col = col_stop - col_start + 1;
	uint8_t offset_row = row_stop - row_start + 1;
	uint16_t total_pixel = offset_col * offset_row;
	while(count < total_pixel)
	{
		if(Send_Data_Byte(ConFig_TFT, high_order_byte) == 0) return 0;
		if(Send_Data_Byte(ConFig_TFT, low_order_byte) == 0) return 0;
		count++;
	}
	return 1;
}

uint8_t Fill_Screen(TFT_HandleTypeDef *ConFig_TFT, uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t count = 0;
	if(Set_Window(ConFig_TFT,0, 0, 159, 79) == 0) return 0;
	uint16_t receive_byte = Packet_Color(red, green, blue);
	uint8_t high_order_byte = receive_byte >> 8;
	uint8_t low_order_byte = receive_byte;
	if(Send_Command_Byte(ConFig_TFT, RAMWR) == 0) return 0;
	uint16_t total_pixel = 160 * 80;
	while(count < total_pixel)
	{
		if(Send_Data_Byte(ConFig_TFT, high_order_byte) == 0) return 0;
		if(Send_Data_Byte(ConFig_TFT, low_order_byte) == 0) return 0;
		count++;
	}
	return 1;
}

static uint8_t Draw_Char(TFT_HandleTypeDef *ConFig_TFT, uint8_t character, uint8_t col_coordinate, uint8_t row_coordinate, uint8_t red, uint8_t green, uint8_t blue)
{
	static const uint8_t array_byte[475] =
	{
		    0x00, 0x00, 0x00, 0x00, 0x00, // 32  ' '
		    0x00, 0x00, 0x5F, 0x00, 0x00, // 33  !
		    0x00, 0x03, 0x00, 0x03, 0x00, // 34  "
		    0x14, 0x7F, 0x14, 0x7F, 0x14, // 35  #
		    0x24, 0x2A, 0x7F, 0x2A, 0x12, // 36  $
		    0x23, 0x13, 0x08, 0x64, 0x62, // 37  %
		    0x36, 0x49, 0x55, 0x26, 0x50, // 38  &
		    0x00, 0x05, 0x03, 0x00, 0x00, // 39  '
		    0x00, 0x1C, 0x22, 0x41, 0x00, // 40  (
		    0x00, 0x41, 0x22, 0x1C, 0x00, // 41  )
		    0x14, 0x08, 0x3E, 0x08, 0x14, // 42  *
		    0x08, 0x08, 0x3E, 0x08, 0x08, // 43  +
		    0x00, 0x50, 0x30, 0x00, 0x00, // 44  ,
		    0x08, 0x08, 0x08, 0x08, 0x08, // 45  -
		    0x00, 0x60, 0x60, 0x00, 0x00, // 46  .
		    0x20, 0x10, 0x08, 0x04, 0x02, // 47  /
		    0x3E, 0x51, 0x49, 0x45, 0x3E, // 48  0
		    0x00, 0x42, 0x7F, 0x40, 0x00, // 49  1
		    0x42, 0x61, 0x51, 0x49, 0x46, // 50  2
		    0x21, 0x41, 0x45, 0x4B, 0x31, // 51  3
		    0x18, 0x14, 0x12, 0x7F, 0x10, // 52  4
		    0x27, 0x45, 0x45, 0x45, 0x39, // 53  5
		    0x3C, 0x4A, 0x49, 0x49, 0x30, // 54  6
		    0x01, 0x71, 0x09, 0x05, 0x03, // 55  7
		    0x36, 0x49, 0x49, 0x49, 0x36, // 56  8
		    0x06, 0x49, 0x49, 0x29, 0x1E, // 57  9
		    0x00, 0x36, 0x36, 0x00, 0x00, // 58  :
		    0x00, 0x56, 0x36, 0x00, 0x00, // 59  ;
		    0x08, 0x14, 0x22, 0x41, 0x00, // 60  <
		    0x14, 0x14, 0x14, 0x14, 0x14, // 61  =
		    0x00, 0x41, 0x22, 0x14, 0x08, // 62  >
		    0x02, 0x01, 0x59, 0x09, 0x06, // 63  ?
		    0x32, 0x49, 0x79, 0x41, 0x3E, // 64  @
		    0x7E, 0x11, 0x11, 0x11, 0x7E, // 65  A
		    0x7F, 0x49, 0x49, 0x49, 0x36, // 66  B
		    0x3E, 0x41, 0x41, 0x41, 0x22, // 67  C
		    0x7F, 0x41, 0x41, 0x22, 0x1C, // 68  D
		    0x7F, 0x49, 0x49, 0x49, 0x41, // 69  E
		    0x7F, 0x09, 0x09, 0x09, 0x01, // 70  F
		    0x3E, 0x41, 0x49, 0x49, 0x7A, // 71  G
		    0x7F, 0x08, 0x08, 0x08, 0x7F, // 72  H
		    0x00, 0x41, 0x7F, 0x41, 0x00, // 73  I
		    0x20, 0x40, 0x41, 0x3F, 0x01, // 74  J
		    0x7F, 0x08, 0x14, 0x22, 0x41, // 75  K
		    0x7F, 0x40, 0x40, 0x40, 0x40, // 76  L
		    0x7F, 0x02, 0x0C, 0x02, 0x7F, // 77  M
		    0x7F, 0x04, 0x08, 0x10, 0x7F, // 78  N
		    0x3E, 0x41, 0x41, 0x41, 0x3E, // 79  O
		    0x7F, 0x09, 0x09, 0x09, 0x06, // 80  P
		    0x3E, 0x41, 0x51, 0x21, 0x5E, // 81  Q
		    0x7F, 0x09, 0x19, 0x39, 0x46, // 82  R
		    0x46, 0x49, 0x49, 0x49, 0x31, // 83  S
		    0x01, 0x01, 0x7F, 0x01, 0x01, // 84  T
		    0x3F, 0x40, 0x40, 0x40, 0x3F, // 85  U
		    0x1F, 0x20, 0x40, 0x20, 0x1F, // 86  V
		    0x3F, 0x40, 0x30, 0x40, 0x3F, // 87  W
		    0x63, 0x14, 0x08, 0x14, 0x63, // 88  X
		    0x07, 0x08, 0x70, 0x08, 0x07, // 89  Y
		    0x61, 0x51, 0x49, 0x45, 0x43, // 90  Z
		    0x00, 0x7F, 0x41, 0x41, 0x00, // 91  [
		    0x02, 0x04, 0x08, 0x10, 0x20, // 92  backslash
		    0x00, 0x41, 0x41, 0x7F, 0x00, // 93  ]
		    0x04, 0x02, 0x01, 0x02, 0x04, // 94  ^
		    0x40, 0x40, 0x40, 0x40, 0x40, // 95  _
		    0x00, 0x01, 0x02, 0x04, 0x00, // 96  `
		    0x20, 0x54, 0x54, 0x54, 0x78, // 97  a
		    0x7F, 0x48, 0x44, 0x44, 0x38, // 98  b
		    0x38, 0x44, 0x44, 0x44, 0x20, // 99  c
		    0x38, 0x44, 0x44, 0x48, 0x7F, // 100 d
		    0x38, 0x54, 0x54, 0x54, 0x18, // 101 e
		    0x08, 0x7E, 0x09, 0x09, 0x02, // 102 f
		    0x0C, 0x52, 0x52, 0x52, 0x3E, // 103 g
		    0x7F, 0x08, 0x04, 0x04, 0x78, // 104 h
		    0x00, 0x44, 0x7D, 0x40, 0x00, // 105 i
		    0x20, 0x40, 0x44, 0x3D, 0x00, // 106 j
		    0x7F, 0x10, 0x28, 0x44, 0x00, // 107 k
		    0x00, 0x41, 0x7F, 0x40, 0x00, // 108 l
		    0x7C, 0x04, 0x78, 0x04, 0x78, // 109 m
		    0x7C, 0x08, 0x04, 0x04, 0x78, // 110 n
		    0x38, 0x44, 0x44, 0x44, 0x38, // 111 o
		    0x7C, 0x14, 0x14, 0x14, 0x08, // 112 p
		    0x08, 0x14, 0x14, 0x18, 0x7C, // 113 q
		    0x7C, 0x08, 0x04, 0x04, 0x08, // 114 r
		    0x48, 0x54, 0x54, 0x54, 0x20, // 115 s
		    0x04, 0x3F, 0x44, 0x40, 0x20, // 116 t
		    0x3C, 0x40, 0x40, 0x20, 0x7C, // 117 u
		    0x1C, 0x20, 0x40, 0x20, 0x1C, // 118 v
		    0x3C, 0x40, 0x30, 0x40, 0x3C, // 119 w
		    0x44, 0x28, 0x10, 0x28, 0x44, // 120 x
		    0x0C, 0x50, 0x50, 0x50, 0x3C, // 121 y
		    0x44, 0x64, 0x54, 0x4C, 0x44, // 122 z
		    0x00, 0x08, 0x36, 0x41, 0x00, // 123 {
		    0x00, 0x00, 0x7F, 0x00, 0x00, // 124 |
		    0x00, 0x41, 0x36, 0x08, 0x00, // 125 }
			0x01, 0x08, 0x01, 0x01, 0x08  // 126 ~
	};
	uint16_t offset = (character - 32) * 5;
	for(uint8_t index_col = 0; index_col <= 4; index_col++)
	{
		uint16_t offset_col = offset + index_col;
		uint8_t  byte_bitmap = array_byte[offset_col];
		for(uint8_t index_row = 0; index_row <= 6; index_row++)
		{
			uint8_t bitmap = byte_bitmap & 1 << index_row;
			if(bitmap != 0)
			{
				if(Draw_Pixel(ConFig_TFT,col_coordinate + index_col, row_coordinate + index_row, red, green, blue) == 0) return 0;
			}
		}
	}
	return 1;
}

uint8_t Print_String(TFT_HandleTypeDef *ConFig_TFT, const char *string, uint8_t col_coordinate, uint8_t row_coordinate, uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t index = 0;
	uint8_t current_character = *(string + index);
	while(current_character != '\0')
	{
		if(Draw_Char(ConFig_TFT,current_character ,col_coordinate, row_coordinate, red, green, blue) == 0) return 0;
		index++;
		col_coordinate += 6;
		current_character = *(string + index);
	}
	return 1;
}
