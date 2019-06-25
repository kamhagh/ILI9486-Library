#ifndef _CLOCK_H
#define _CLOCK_H

#include <lpc17xx.h>

#define PLOCK 0x400 //10th bit = 1

void feedSeq();
void Clock_Init(uint8_t n, uint8_t m, uint8_t d);

#endif