#include "ILI9486.h"
#include "Font_24x16.h"

int16_t textColor = White, BackgroundColor = Black;
int16_t fontSize = 1;

__asm void wait()
{
    nop
    BX lr
}

void SWAP(int16_t* a, int16_t* b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}
void TFT_Init() {
	TFT_ControlPort->FIODIR |= PIN_CS | PIN_RD | PIN_RS | PIN_RST | PIN_WR;
	TFT_DataPort->FIODIR |= (0xFF << LowNibbleOffset) | (0xFF << HighNibbleOffset);
	
	TFT_ControlPort->FIOSET |= PIN_CS | PIN_RD | PIN_RS | PIN_RST | PIN_WR;
	
	delayms(1);
	LCD_RST(0);
	delayms(10);             
	LCD_RST(1);
	delayms(100);
	
	TFT_WriteCMD(0x11); // Exit sleep mode
	delayms(120);

	TFT_WriteCMD(0xD0);    //Power setting
	TFT_WriteData(0x07);  	// 1xVci
	TFT_WriteData(0x42);
	TFT_WriteData(0x18);

	TFT_WriteCMD(0xD1);	 // Vcom control
	TFT_WriteData(0x00);
	TFT_WriteData(0x07);	//07
	TFT_WriteData(0x10);

	TFT_WriteCMD(0xD2);	// Power setting for normal mode 
	TFT_WriteData(0x01);
	TFT_WriteData(0x02);

	TFT_WriteCMD(0xC0);	// Panel driving settings
	TFT_WriteData(0x10);
	TFT_WriteData(0x3B);
	TFT_WriteData(0x00);
	TFT_WriteData(0x02);
	TFT_WriteData(0x11);

	TFT_WriteCMD(0xC5);	// Frame rate and inversion control
	TFT_WriteData(0x03);	// 72 Hz

	TFT_WriteCMD(0xC8);		// Gamma settings
	TFT_WriteData(0x00);
	TFT_WriteData(0x32);
	TFT_WriteData(0x36);
	TFT_WriteData(0x45);
	TFT_WriteData(0x06);
	TFT_WriteData(0x16);
	TFT_WriteData(0x37);
	TFT_WriteData(0x75);
	TFT_WriteData(0x77);
	TFT_WriteData(0x54);
	TFT_WriteData(0x0C);
	TFT_WriteData(0x00);

	TFT_WriteCMD(0x36);		// set address mode
	TFT_WriteData(0x0A);	

	TFT_WriteCMD(0x3A);		// set pixel format
	TFT_WriteData(0x55);

	TFT_WriteCMD(0x2A);		// set column address
	TFT_WriteData(0x00);
	TFT_WriteData(0x00);
	TFT_WriteData(0x01);
	TFT_WriteData(0x3F);

	TFT_WriteCMD(0x2B);		// set page address
	TFT_WriteData(0x00);
	TFT_WriteData(0x00);
	TFT_WriteData(0x01);
	TFT_WriteData(0xE0);

	delayms(120);

	TFT_WriteCMD(0x29);
	TFT_WriteCMD(0x2C);

}

void TFT_SetFontSize(uint16_t sizeFont)
{
	fontSize = sizeFont;
}

void TFT_DrawString(int16_t x, int16_t y, char *string, int16_t color)
{
	int i;
	for(i = 0; string[i] != 0; i++) {
		TFT_DrawChar(x + (8 * i * fontSize), y , string[i], White);
	}
}

void TFT_DrawButton(int16_t x, int16_t y, char *string, int16_t color, int16_t BackgroundColor)
{
	int c;
	while(string[c] != '\0')
      c++;
	TFT_DrawFillRect(x, y, x + (9 * fontSize * c) + 20, y + 9 + 40, BackgroundColor);
	TFT_setBackgroundColor(BackgroundColor);
	TFT_DrawString(x + 20, y + 20, string, color);
}

void TFT_DrawChar(int16_t x, int16_t y, char character, int16_t color)
{
	int16_t i, j, k, h;
	
	LCD_Window(x, x + (8 * fontSize) -1, y, y + (8 * fontSize) -1);
	
	TFT_StartWrite();
	for(int i = 0; i < 8; i++) {
		for(h = 0; h < fontSize; h++) {
			for(int j = 0; j < 8; j++) {
				if((font8x8_basic[character][j] >> i) & 0x1) {
					for(k = 0; k < fontSize; k++)
						TFT_SendData(color);
					} else {
					for(k = 0; k < fontSize; k++)
						TFT_SendData(BackgroundColor);
				}
			}
		}
	}
}

void TFT_setTextColor(int16_t color)
{
	textColor = color;
}

void TFT_setBackgroundColor(int16_t color)
{
	BackgroundColor = color;
}

void TFT_Clear (int16_t color) 
{
	unsigned int   i;

	LCD_WindowMax();

	TFT_StartWrite();
	for(i = 0; i < (WIDTH*HEIGHT); i++) {
		TFT_SendData(color);
	}
	TFT_Show();
}

void TFT_DrawFillRect(int16_t startx, int16_t starty, int16_t endx, int16_t endy, int16_t color)
{
	unsigned int i;
	unsigned int sizerect = (endx - startx) * (endy - starty);
	LCD_Window(startx, endx-1, starty, endy-1);
	
	TFT_StartWrite();
	for(i=0; i < sizerect; i++) {
		TFT_SendData(color);
	}
}

void TFT_DrawPixel(int16_t x, int16_t y, int16_t color)
{
	LCD_Window(x, x, y, y);
	
	TFT_StartWrite();
	TFT_SendData(color);
}

void TFT_DrawCircle(int16_t x0, int16_t y0, int16_t r, int16_t color)
{
	{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        TFT_DrawPixel(x0-x, y0+y,color);
        TFT_DrawPixel(x0+x, y0+y,color);
        TFT_DrawPixel(x0+x, y0-y,color);
        TFT_DrawPixel(x0-x, y0-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
	}
}

void TFT_DrawFillCircle(int16_t x0, int16_t y0, int16_t r, int16_t color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        TFT_DrawLineVertical(y0-y, y0+y, x0-x, color);
        TFT_DrawLineVertical(y0-y, y0+y, x0+x, color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    } while (x <= 0);
}

void TFT_DrawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color)
{
	if (x1 > x0) TFT_DrawLineHorizontal(x0,x1,y0,color);
	else  TFT_DrawLineHorizontal(x1,x0,y0,color);

	if (y1 > y0) TFT_DrawLineVertical(y0,y1, x0, color);
	else TFT_DrawLineVertical(y1,y0, x0, color);

	if (x1 > x0) TFT_DrawLineHorizontal(x0,x1,y1,color);
	else  TFT_DrawLineHorizontal(x1,x0,y1,color);

	if (y1 > y0) TFT_DrawLineVertical(y0,y1, x1, color);
	else TFT_DrawLineVertical(y1,y0, x1, color);
	return;
}

void TFT_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color)
{
    //WindowMax();
    int16_t   dx = 0, dy = 0;
    int16_t   dx_sym = 0, dy_sym = 0;
    int16_t   dx_x2 = 0, dy_x2 = 0;
    int16_t   di = 0;
 
    dx = x1-x0;
    dy = y1-y0;
 
    if (dx == 0) {        /* vertical line */
        if (y1 < y0) SWAP(y0,y1);
        TFT_DrawLineVertical(x0,y0,y1,color);
        return;
    }
 
    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    if (dy == 0) {        /* horizontal line */
        if (x1 < x0) SWAP(x1,x0);
        TFT_DrawLineHorizontal(x0,x1,y0,color);
        return;
    }
 
    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }
 
    dx = dx_sym*dx;
    dy = dy_sym*dy;
 
    dx_x2 = dx*2;
    dy_x2 = dy*2;
 
    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {
 
            TFT_DrawPixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        TFT_DrawPixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
            TFT_DrawPixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        TFT_DrawPixel(x0, y0, color);
    }
    return;
}

void TFT_DrawLineHorizontal(int16_t startx, int16_t endx, int16_t y, int16_t color)
{
	int16_t i;
	
	LCD_Window(startx, endx, y, y);
	
	TFT_StartWrite();
	for(i = startx; i <= endx ; i++)
	{
		TFT_SendData(color);
	}
}

void TFT_DrawLineVertical(int16_t starty, int16_t endy, int16_t x, int16_t color)
{
	int16_t i;
	
	LCD_Window(x, x, starty, endy);
	
	TFT_StartWrite();
	for(i = starty; i <= endy ; i++)
	{
		TFT_SendData(color);
	}
}

static __inline void LCD_WindowMax()
{
	LCD_Window(0, WIDTH-1, 0, HEIGHT-1);
}

void LCD_Window(int16_t Xstart, int16_t Xend, int16_t Ystart, int16_t Yend) 
{
	TFT_WriteCMD(0x2a);   
	TFT_WriteData(Ystart>>8);
	TFT_WriteData(Ystart&0xff);
	TFT_WriteData(Yend>>8);
	TFT_WriteData(Yend&0xff);

	TFT_WriteCMD(0x2b);   
	TFT_WriteData(Xstart>>8);
	TFT_WriteData(Xstart&0xff);
	TFT_WriteData(Xend>>8);
	TFT_WriteData(Xend&0xff);

	//TFT_WriteCMD(0x3A);    // 16/18 bits
    //TFT_WriteData(0x55);
	
	TFT_WriteCMD(0x2c);
}

void TFT_Show() {
	LCD_CS(1);
	LCD_WindowMax();
}

void TFT_DrawBitmap(unsigned short *image)
{
	unsigned int i;
	LCD_Window(0, WIDTH-1, 0, HEIGHT-1);

	TFT_StartWrite();
	for(i = 0; i < (WIDTH*HEIGHT); i++) {
		TFT_SendData(image[i]);
	}
}
static __inline void TFT_SendData(int16_t data){
	LCD_WR(0)
	TFT_SetData(data);
	LCD_WR(1);
}

static __inline void TFT_WriteCMD(uint16_t data) { //WR: rise, RD: H, RS(D/CX):L
	LCD_CS(0);
	LCD_RS(0);
	LCD_WR(0);
	TFT_SetData(data);
	LCD_WR(1);
	LCD_RS(1);
	LCD_CS(1);
}

static __inline void TFT_WriteData(uint16_t data) {
	LCD_CS(0);
	LCD_RS(1);
	LCD_WR(0);
	TFT_SetData(data);
	LCD_WR(1);
	LCD_RS(1);
	LCD_CS(1);
}

static __inline int16_t TFT_ReadData() { //
	unsigned short val = 0;
	LCD_RD(1)
	LCD_CS(0)
	LCD_RS(1)
	LCD_WR(1)
	LCD_RD(0)
	// Add delay if having issues? (3)
	val = TFT_ReadPins();
	LCD_RD(1)
	// add delay (3) if having issues
	LCD_CS(1)
	return val;
}

// Set data to the GPIO pins (assumes the fiodir is output, maybe set a flag for input mode or output mode alter to speed up)
static __inline void TFT_SetData(int16_t data) {
	TFT_DataPort->FIOCLR |= 0xFF << LowNibbleOffset | 0xFF << HighNibbleOffset; // Clear the bits.
	
	TFT_DataPort->FIOSET |= ((data >> 8) << HighNibbleOffset) | ((data & 0xFF) << LowNibbleOffset); // Set High and Low bit.
}

static __inline void TFT_StartWrite()
{
	LCD_CS(0);
	LCD_RS(1);
	LCD_RD(1);
}

static __inline int16_t TFT_ReadPins()
{
	int16_t data;
	TFT_DataPort-> FIODIR &= (~( 0xFF << LowNibbleOffset)) |  (~(0xFF << HighNibbleOffset));
	
	data = TFT_DataPort->FIOPIN >> HighNibbleOffset & 0xFF;
	data <<= 8;
	data |= TFT_DataPort->FIOPIN >> LowNibbleOffset & 0xFF;
	
	return data;
}

static __inline int16_t TFT_ReadStatus() { //Implement later
	
}

