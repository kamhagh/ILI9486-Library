

#include "Delay.h"

void Delay_init()
{
	//Turn on power, 1 for timer 0, 2 for timer 1, 22 for timer 2, 23 for timer 3.
	LPC_SC->PCONP |= 1 << 1;
	//Set Clock/4
	LPC_SC->PCLKSEL0 &= ~(0x3 << 2);
	
	//Set Timer0 to timer mode
	LPC_TIM0->CTCR = 0x0;
	
	//setPrescaler
	LPC_TIM0->PR = getPrescalar(2);
	
	LPC_TIM0->TCR = 0x02;
}

void delayms(unsigned int milliseconds) //Using Timer0
{
	LPC_TIM0->TCR = 0x02; //Reset Timer

	LPC_TIM0->TCR = 0x01; //Enable timer

	while(LPC_TIM0->TC < milliseconds*1000); //wait until timer counter reaches the desired delay

	LPC_TIM0->TCR = 0x00; //Disable timer
}

void delayus(unsigned int nanoseconds) //Using Timer0
{
	LPC_TIM0->TCR = 0x02; //Reset Timer

	LPC_TIM0->TCR = 0x01; //Enable timer

	while(LPC_TIM0->TC < nanoseconds); //wait until timer counter reaches the desired delay

	LPC_TIM0->TCR = 0x00; //Disable timer
}

unsigned int getPrescalar(uint8_t timerPclkBit)
{
    unsigned int pclk,prescalarForUs;
    pclk = (LPC_SC->PCLKSEL0 >> timerPclkBit) & 0x03; /* get the pclk info for required timer */

    switch ( pclk )                                   /* Decode the bits to determine the pclk*/
    {
    case 0x00:
        pclk = SystemCoreClock/4;
        break;

    case 0x01:
        pclk = SystemCoreClock;
        break; 

    case 0x02:
        pclk = SystemCoreClock/2;
        break; 

    case 0x03:
        pclk = SystemCoreClock/8;
        break;
    }

    prescalarForUs =pclk/1000000 - 1;                 /* Prescalar for 1us (1000000Counts/sec) */

    return prescalarForUs;
}
