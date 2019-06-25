#ifndef _DELAY_H_
#define _DELAY_H_

#include <lpc17xx.h>

void Delay_init();
void delayms(unsigned int milliseconds);
void delayus(unsigned int nanoseconds);
unsigned int getPrescalar(uint8_t timerPclkBit);

#endif