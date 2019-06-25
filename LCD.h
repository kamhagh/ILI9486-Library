#ifndef _LCD_H_
#define _LCD_H_

#include "GPIO.h"

void LCD_Init(uint8_t rs, uint8_t rw, uint8_t e, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
void LCD_SetHalf(char cmd);
void LCD_PulseEN();
void LCD_SendCommand(char cmd);
void LCD_SendChar(char msg);
void LCD_printf(char *msg);
void LCD_Clear();

#endif