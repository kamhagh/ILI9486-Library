/* Copyright (C) 2010, 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

/* see chapter 4 of user manual for details on clocks and power */

#ifndef FASTLIB_POWER_H
#define FASTLIB_POWER_H

#include "fastlib/common.h"

#define FL_PCON     ((volatile uint32_t *) 0x400FC0C0)
#define FL_PCONP    ((volatile uint32_t *) 0x400FC0C4)

#define FL_POWER_DOWN                       (1<<0)
#define FL_DEEP_POWER_DOWN                  (1<<1)
#define FL_BROWN_OUT_REDUCED_POWER_MODE     (1<<2)
#define FL_BROWN_OUT_GLOBAL_DISABLE         (1<<3)
#define FL_BROWN_OUT_RESET_DISABLE          (1<<4)

#define FL_FLAG_SLEEP_MODE                  (1<<8)
#define FL_FLAG_DEEP_SLEEP_MODE             (1<<9)
#define FL_FLAG_POWER_DOWN                  (1<<10)
#define FL_FLAG_DEEP_POWER_DOWN             (1<<11)

static inline void fl_set_power_mode(const unsigned mode) {
    *FL_PCON = mode;
}

static inline unsigned fl_get_power_mode(void) {
    return *FL_PCON;
}

#define FL_POWER(peripheral, pos) \
    static inline void fl_power_##peripheral(const unsigned on_off) { \
        *FL_PCONP &= ~(              1U<<pos); \
        *FL_PCONP |=   (unsigned)on_off<<pos; \
    }

FL_POWER(timer0, 1)
FL_POWER(timer1, 2)
FL_POWER(uart0,  3)
FL_POWER(uart1,  4)
FL_POWER(pwm1,   6)
FL_POWER(i2c0,   7)
FL_POWER(spi,    8)
FL_POWER(rtc,    9)
FL_POWER(ssp1,   10)
FL_POWER(adc,    12)
FL_POWER(can1,   13)
FL_POWER(can2,   14)
FL_POWER(gpio,   15)
FL_POWER(rit,    16)
FL_POWER(mcpwm,  17)
FL_POWER(qei,    18)
FL_POWER(i2c1,   19)
FL_POWER(ssp0,   21)
FL_POWER(timer2, 22)
FL_POWER(timer3, 23)
FL_POWER(uart2,  24)
FL_POWER(uart3,  25)
FL_POWER(i2c2,   26)
FL_POWER(i2s,    27)
FL_POWER(gpdma,  29)
FL_POWER(enet,   30)
FL_POWER(usb,    31)

static inline void fl_power_off_all_peripherals(void) {
    *FL_PCONP = 0;
}

static inline void fl_power_on_all_peripherals(void) {
    *FL_PCONP = ~((1<<0) | (1<<5) | (1<<11) | (1<<20) | (1<<28));
}

#endif