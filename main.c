#include <lpc17xx.h>
#include "Delay.h"
#include "Clock.h"
#include "ILI9486.h"
#include "stdlib.h"

#define SquareSize 40

int i,j;

int main()
{
	SystemInit();

	Delay_init();
	
	TFT_Init();
	
	LPC_GPIO2->FIODIR |= 1 << 0;
	LPC_GPIO1->FIODIR &= ~(1 << 27 | 1 << 28);
	LPC_GPIO2->FIOSET |= 1 << 0;
	
	TFT_Clear(Black);

	TFT_setBackgroundColor(Black);
	TFT_DrawButton(80,80, "Cute Demo!", White, Navy);
	
	while (1)
	{
		
		if(!(LPC_GPIO1->FIOPIN & (1 << 27))) {
			TFT_Clear(Black);
			TFT_DrawButton(80,80, "I Love abbi ;3 <3", White, Purple);
			TFT_Show();
		} else if ((LPC_GPIO1->FIOPIN & (1 << 28))) {
			TFT_Clear(Black);
			TFT_DrawButton(80,80, "I Love Sarah ;3 <3", White, Blue);
			TFT_Show();
		}
//		for(i = 60; i < 400; i++)
//		{
//			TFT_DrawFillCircle(i, 150, 50, Purple);
//			TFT_Show();
//			delayms(3);
//		}
//		
//		TFT_DrawFillCircle(400, 150, 50, Purple);
//		delayms(500);
//		
//		for(i = 400; i >= 60; i--)
//		{
//			TFT_DrawFillCircle(i, 150, 50, Black);
//			delayms(3);
//		}
		
		
		
//		for(i = 0; i < 320; i+=SquareSize) {
//			for(j=0; j <480; j+=SquareSize) {
//				TFT_DrawRect(i, j, i+SquareSize, j+SquareSize, rand() % 0xFFFF);
//			}
//		}
//		TFT_Show();
//		delayms(1000);
	}
}
