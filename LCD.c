#include "LCD.h"
#include "GPIO.h"
#include "Delay.h"

#define Delay() delayus(100);
#define shortDelay() delayus(1);
#define longDelay() delayms(5);

uint8_t Pin_RS;
uint8_t Pin_RW;
uint8_t Pin_E;
uint8_t Pin_D4;
uint8_t Pin_D5;
uint8_t Pin_D6;
uint8_t Pin_D7;


void LCD_Init(uint8_t rs, uint8_t rw, uint8_t e, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	//Set Pin Numbers
	Pin_RW = rw;
	Pin_E = e;
	Pin_RS = rs;
	Pin_D4 = d4;
	Pin_D5 = d5;
	Pin_D6 = d6;
	Pin_D7 = d7;
	
	//Set port Directions
	GPIO_PinDirection(Pin_D4, 1);
	GPIO_PinDirection(Pin_D5, 1);
	GPIO_PinDirection(Pin_D6, 1);
	GPIO_PinDirection(Pin_D7, 1);
	GPIO_PinDirection(Pin_RW, 1);
	GPIO_PinDirection(Pin_RS, 1);
	GPIO_PinDirection(Pin_E, 1);
	
	longDelay();
	
	//init display in 4 bit mode
	LCD_SendCommand(0x02);                   // Initialize Lcd in 4-bit mode
	LCD_SendCommand(0x28);                   // enable 5x7 mode for chars 
	LCD_SendCommand(0x0c);                   // Display OFF, Cursor ON
	LCD_SendCommand(0x01);                   // Clear Display
	longDelay();
	LCD_SendCommand(0x80);                   // Move the cursor to beginning of first line
}

void LCD_printf(char *msg)
{
	while(*msg != 0) {
		LCD_SendChar(*msg++);
	}
}

void LCD_SetHalf(char cmd)
{
	GPIO_PinWrite(Pin_D4, ((cmd >> 0) & 0x01));
	GPIO_PinWrite(Pin_D5, ((cmd >> 1) & 0x01));
	GPIO_PinWrite(Pin_D6, ((cmd >> 2) & 0x01));
	GPIO_PinWrite(Pin_D7, ((cmd >> 3) & 0x01));
}

void LCD_SendCommand(char cmd)
{
	LCD_SetHalf(cmd >> 0x04); // Higher half
	GPIO_PinWrite(Pin_RS, 0);
	GPIO_PinWrite(Pin_RW, 0);
	LCD_PulseEN();
	
	Delay();
	
	LCD_SetHalf(cmd & 0x0F);
	GPIO_PinWrite(Pin_RS, 0);
	GPIO_PinWrite(Pin_RW, 0);
	LCD_PulseEN();
	
	Delay();
}

void LCD_SendChar(char msg)
{
	LCD_SetHalf(msg >> 0x04); // Higher half
	GPIO_PinWrite(Pin_RS, 1);
	GPIO_PinWrite(Pin_RW, 0);
	LCD_PulseEN();
	
	Delay();
	
	LCD_SetHalf(msg & 0x0F);
	GPIO_PinWrite(Pin_RS, 1);
	GPIO_PinWrite(Pin_RW, 0);
	LCD_PulseEN();
	
	Delay();
}

void LCD_Clear()
{
	LCD_SendCommand(0x01);
	longDelay();
	LCD_SendCommand(0x2);
	longDelay();
}

void LCD_PulseEN()
{
	GPIO_PinWrite(Pin_E, 1);
	shortDelay();
	GPIO_PinWrite(Pin_E, 0);
}