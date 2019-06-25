#ifndef __ILI9486_H__
#define __ILI9486_H__

#include "Delay.h"

#ifndef TFT_DataPort
#define TFT_DataPort LPC_GPIO0
#endif

#ifndef LowNibbleOffset // Define lower 8 Bit pins.
#define LowNibbleOffset 4
#endif

#ifndef HighNibbleOffset // Define higher 8 bit pins.
#define HighNibbleOffset 15
#endif


#ifndef TFT_ControlPort
#define TFT_ControlPort LPC_GPIO2
#endif

#ifndef PIN_CS
#define PIN_CS (1UL << 8)
#endif

#ifndef PIN_RS
#define PIN_RS			(1UL << 9)
#endif

#ifndef PIN_WR
#define PIN_WR			(1UL << 10)
#endif

#ifndef PIN_RD
#define PIN_RD			(1UL << 11) 
#endif

#ifndef PIN_RST
#define PIN_RST			(1UL << 13)
#endif

#ifndef WIDTH
#define WIDTH       	480         		/* Screen Width (in pixels)           */
#define HEIGHT      	320             /* Screen Hight (in pixels)           */
#endif

#define LCD_CS(x)     	((x) ? (TFT_ControlPort->FIOSET = PIN_CS) : (TFT_ControlPort->FIOCLR = PIN_CS));
#define LCD_RS(x)     	((x) ? (TFT_ControlPort->FIOSET = PIN_RS) : (TFT_ControlPort->FIOCLR = PIN_RS));
#define LCD_WR(x)     	((x) ? (TFT_ControlPort->FIOSET = PIN_WR) : (TFT_ControlPort->FIOCLR = PIN_WR));
#define LCD_RD(x)     	((x) ? (TFT_ControlPort->FIOSET = PIN_RD) : (TFT_ControlPort->FIOCLR = PIN_RD));
#define LCD_RST(x)    	((x) ? (TFT_ControlPort->FIOSET = PIN_RST): (TFT_ControlPort->FIOCLR = PIN_RST));

/* LCD RGB color definitions */
#define Black           0x0000			/*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255, 0   */
#define White           0xFFFF      /* 255, 255, 255 */
#define WHITE           0XFFFF      /* 255, 255, 255 */   
#define RED             0xF800      /* 255,   0,   0 */
#define BLUE            0x001F      /*   0,   0, 255 */

void TFT_Init(); //Initialization
void TFT_Clear (int16_t color);
void TFT_WriteCMD(uint16_t Data);
void TFT_StartWrite();
void TFT_WriteData(uint16_t data);
void TFT_SendData(int16_t data);
void TFT_SetData(int16_t data); // Set data to the GPIO ports.
void LCD_Window(int16_t Xstart, int16_t Xend, int16_t Ystart, int16_t Yend);
void LCD_WindowMax();
void TFT_setTextColor(int16_t color);
void TFT_setBackgroundColor(int16_t color);
void TFT_DisplayChar (unsigned int ln, unsigned int col, unsigned char c);
void TFT_Show();

void TFT_DrawPixel(int16_t x, int16_t y, int16_t color);

void TFT_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color);
void TFT_DrawLineVertical(int16_t starty, int16_t endy, int16_t x, int16_t color);
void TFT_DrawLineHorizontal(int16_t startx, int16_t endx, int16_t y, int16_t color);

void TFT_DrawCircle(int16_t x0, int16_t y0, int16_t r, int16_t color);
void TFT_DrawFillCircle(int16_t x0, int16_t y0, int16_t r, int16_t color);

void TFT_DrawFillRect(int16_t startx, int16_t starty, int16_t endx, int16_t endy, int16_t color);  
void TFT_DrawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color);

void TFT_DrawChar(int16_t x, int16_t y, char character, int16_t color);
void TFT_SetFontSize(uint16_t sizeFont);
void TFT_DrawString(int16_t x, int16_t y, char *string, int16_t color);
void TFT_DrawButton(int16_t x, int16_t y, char *string, int16_t color, int16_t BackgroundColor);

void TFT_DrawBitmap(unsigned short *image);
int16_t TFT_ReadPins();
int16_t TFT_ReadData();
int16_t TFT_ReadStatus();





#endif