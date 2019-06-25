/* Copyright (C) 2010, 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */
 
/* see chapter 4 of user manual for details on clocks and power */

#ifndef FASTLIB_CLKPWR_H
#define FASTLIB_CLKPWR_H

#include "fastlib/common.h"

#define FL_CLKSRCSEL    ((volatile uint32_t *) 0x400FC10C)

#define FL_PLL0CON      ((volatile uint32_t *) 0x400FC080)
#define FL_PLL0CFG      ((volatile uint32_t *) 0x400FC084)
#define FL_PLL0STAT     ((volatile uint32_t *) 0x400FC088)
#define FL_PLL0FEED     ((volatile uint32_t *) 0x400FC08C)

#define FL_PLL1CON      ((volatile uint32_t *) 0x400FC0A0)
#define FL_PLL1CFG      ((volatile uint32_t *) 0x400FC0A4)
#define FL_PLL1STAT     ((volatile uint32_t *) 0x400FC0A8)
#define FL_PLL1FEED     ((volatile uint32_t *) 0x400FC0AC)

#define FL_CCLKCFG      ((volatile uint32_t *) 0x400FC104)
#define FL_USBCLKCFG    ((volatile uint32_t *) 0x400FC108)
#define FL_PCLKSEL0     ((volatile uint32_t *) 0x400FC1A8)
#define FL_PCLKSEL1     ((volatile uint32_t *) 0x400FC1AC)

#define FL_CLKOUTCFG    ((volatile uint32_t *) 0x400FC1C8)

/* Internal RC oscillator, nominal frequency is 4MHz
 * Main oscillator --> mbed: 12MHz slave mode
 * RTC oscillator --> mbed: FC-135 32.7680kHz 12.5pF
 */

/* for USB, use only Main oscillator
 * for CAN over 100 kbit/s, do not use Internal RC oscillator
 */

#define FL_PLL0_CLOCK_SOURCE_INTERNAL   0
#define FL_PLL0_CLOCK_SOURCE_MAIN       1
#define FL_PLL0_CLOCK_SOURCE_RTC        2

static inline void fl_select_pll0_clock_source(const int source) {
    *FL_CLKSRCSEL = source;
}

/* PLL0:
 * input source --> PLL input divider (N) --> PLL multiplier (M)
 *
 * input:   32kHz-50MHz     -->
 * N:       1-256           -->
 * M:       6-512           --> 275MHz-550MHz
 *
 * PLL0 output dividers below
 */

static inline void fl_pll0_control(const int enable, const int connect) {
    *FL_PLL0CON = enable | (connect<<1);
}

static inline void fl_pll0_config(const unsigned int N, const unsigned int M) {
    *FL_PLL0CFG = (M-1) | ((N-1)<<16);
}

static inline unsigned fl_pll0_get_M(void) {
    return (*FL_PLL0STAT & 0x7fff) + 1;
}

static inline unsigned fl_pll0_get_N(void) {
    return ((*FL_PLL0STAT >> 16) & 0xff) + 1;
}

static inline int fl_pll0_get_enable(void) {
    return *FL_PLL0STAT & (1U<<24);
}

static inline int fl_pll0_get_connect(void) {
    return *FL_PLL0STAT & (1U<<25);
}

static inline int fl_pll0_get_lock(void) {
    return *FL_PLL0STAT & (1U<<26);
}

static inline void fl_pll0_feed(void) {
    *FL_PLL0FEED = 0xaa;
    *FL_PLL0FEED = 0x55;
}

/* PLL1:
 * input main oscillator --> PLL multiplier (M) --> output divider (2P)
 */

static inline void fl_pll1_control(const int enable, const int connect) {
    *FL_PLL1CON = enable | (connect<<1);
}

static inline void fl_pll1_config(const unsigned int M, const unsigned int P) {
    *FL_PLL1CFG = M | (P<<5);
}

static inline int fl_pll1_get_M(void) {
    return *FL_PLL1STAT & 0x1f;
}

static inline int fl_pll1_get_P(void) {
    return (*FL_PLL1STAT >> 5) & 3;
}

static inline int fl_pll1_get_enable(void) {
    return *FL_PLL1STAT & (1U<<8);
}

static inline int fl_pll1_get_connect(void) {
    return *FL_PLL1STAT & (1U<<9);
}

static inline int fl_pll1_get_lock(void) {
    return *FL_PLL1STAT & (1U<<10);
}

static inline void fl_pll1_feed(void) {
    *FL_PLL1FEED = 0xaa;
    *FL_PLL1FEED = 0x55;
}

/* PLL0 output --> CPU Clock */

/* CPU clock must be at least 18MHz for USB to function */

/* div: 3-256 */
static inline void fl_set_cpu_clock_divider(const int div) {
    *FL_CCLKCFG = div - 1;
}

static inline int fl_get_cpu_clock_divider(void) {
    return *FL_CCLKCFG + 1;
}

/* use only if USBPLL/PLL1 is not connected */
/* div: 1-16 (only 6, 8 and 10 produce even number multiples */
static inline void fl_set_pll0_usb_divider(const unsigned int div) {
    *FL_USBCLKCFG = div - 1;
}

static inline int fl_get_pll0_usb_divider(void) {
    return *FL_USBCLKCFG + 1;
}

#define FL_CLOCK_DIV4   0
#define FL_CLOCK_DIV1   1
#define FL_CLOCK_DIV2   2
#define FL_CLOCK_DIV8   3

#define FL_SELECT_CLOCK(peripheral, reg, pos) \
    static inline void fl_select_clock_##peripheral(const unsigned int clock) { \
    *reg &= ~(3U << pos); \
    *reg |= clock << pos; \
}

FL_SELECT_CLOCK(wdt,     FL_PCLKSEL0, 0)
FL_SELECT_CLOCK(timer0,  FL_PCLKSEL0, 2)
FL_SELECT_CLOCK(timer1,  FL_PCLKSEL0, 4)
FL_SELECT_CLOCK(uart0,   FL_PCLKSEL0, 6)
FL_SELECT_CLOCK(uart1,   FL_PCLKSEL0, 8)
FL_SELECT_CLOCK(pwm1,    FL_PCLKSEL0, 12)
FL_SELECT_CLOCK(i2c0,    FL_PCLKSEL0, 14)
FL_SELECT_CLOCK(spi,     FL_PCLKSEL0, 16)
FL_SELECT_CLOCK(ssp1,    FL_PCLKSEL0, 20)
FL_SELECT_CLOCK(dac,     FL_PCLKSEL0, 22)
FL_SELECT_CLOCK(adc,     FL_PCLKSEL0, 24)
FL_SELECT_CLOCK(can1,    FL_PCLKSEL0, 26)
FL_SELECT_CLOCK(can2,    FL_PCLKSEL0, 28)
FL_SELECT_CLOCK(acf,     FL_PCLKSEL0, 30)

FL_SELECT_CLOCK(qei,     FL_PCLKSEL1, 0)
FL_SELECT_CLOCK(gpioint, FL_PCLKSEL1, 2)
FL_SELECT_CLOCK(pcb,     FL_PCLKSEL1, 4)
FL_SELECT_CLOCK(i2c1,    FL_PCLKSEL1, 6)
FL_SELECT_CLOCK(ssp0,    FL_PCLKSEL1, 10)
FL_SELECT_CLOCK(timer2,  FL_PCLKSEL1, 12)
FL_SELECT_CLOCK(timer3,  FL_PCLKSEL1, 14)
FL_SELECT_CLOCK(uart2,   FL_PCLKSEL1, 16)
FL_SELECT_CLOCK(uart3,   FL_PCLKSEL1, 18)
FL_SELECT_CLOCK(i2c2,    FL_PCLKSEL1, 20)
FL_SELECT_CLOCK(i2s,     FL_PCLKSEL1, 22)
FL_SELECT_CLOCK(rit,     FL_PCLKSEL1, 26)
FL_SELECT_CLOCK(syscon,  FL_PCLKSEL1, 28)
FL_SELECT_CLOCK(mcpwm,   FL_PCLKSEL1, 30)

#define FL_CLOCK_OUT_CPU    0
#define FL_CLOCK_OUT_MAIN   1
#define FL_CLOCK_OUT_IRC    2
#define FL_CLOCK_OUT_USB    3
#define FL_CLOCK_OUT_RTC    4

static inline void fl_set_output_clock(const int clock) {
    *FL_CLKOUTCFG = clock;
}

#endif