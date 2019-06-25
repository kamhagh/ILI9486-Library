#include "clock.h"

void Clock_Init(uint8_t n, uint8_t m, uint8_t d)
{

	LPC_SC->CLKSRCSEL = 1;
	LPC_SC->CLKSRCSEL &= ~(1 << 1);
	
	//disable PPL0
	LPC_SC->PLL0CON = 0x0;
	feedSeq();
	//Optional, change cpu divider setting to speed up operation
	//write to Clock Source Select Register if needed
	//write to PPL0CFG
	LPC_SC->PLL0CFG = ((m-1) | ((n-1) << 16));
	feedSeq();
	//enable PPL0
	LPC_SC->PLL0CON = 0x1;
	feedSeq();
	LPC_SC->CCLKCFG=d-1;
	//change CPU divider setting for operation with ppl-
	//wait for ppl0to achieve lock by monitoring PLOCK0 bit in PPL0STAT or using an interrut
	while((LPC_SC->PLL0STAT >> 26) & 0x1);
	//connect PPL0
	LPC_SC->PLL0CON |= 0x2;
	feedSeq();
}

void feedSeq()
{
	__disable_irq();
	LPC_SC->PLL0FEED = 0xAA;
	LPC_SC->PLL0FEED = 0x55;
	__enable_irq();
}