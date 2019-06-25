/* Copyright (C) 2010, 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_PINSEL_H
#define FASTLIB_PINSEL_H

#include "fastlib/common.h"

#define FL_PINSEL0      ((volatile uint32_t *) 0x4002c000)
#define FL_PINMODE0     ((volatile uint32_t *) 0x4002c040)
#define FL_PINMODE_OD0  ((volatile uint32_t *) 0x4002c068)

#define FL_PULLUP       0
#define FL_REPEATER     1
#define FL_TRISTATE     2
#define FL_FLOATING     2
#define FL_PULLDOWN     3

#define FL_FUNC_DEFAULT 0
#define FL_FUNC_ALT1    1
#define FL_FUNC_ALT2    2
#define FL_FUNC_ALT3    3

/* fl_pinsel(port,pin,func,resistor,opendrain) :
 *
 * port         0-4
 * pin          0-31
 * func         0-3     (-1 is ignore)  default/alt1/alt2/alt3
 * resistor     0-3     (-1 is ignore)  pull-up, repeater, tri-state, pull-down
 * opendrain    0-1     (-1 is ignore)  off, on
 */

static inline void fl_pinsel(unsigned port, unsigned pin, const int func, const int resistor, const int opendrain) {
    if (opendrain >= 0) {
        *(FL_PINMODE_OD0 + port) &= ~(       1U << pin);
        *(FL_PINMODE_OD0 + port) |=   opendrain << pin ;
    }

    if (func == -1 && resistor == -1) return;

    port <<= 1;
    if (pin >= 16) {
        pin -= 16;
        port++;
    }
    pin <<= 1;

    if (func >= 0) {    
        *(FL_PINSEL0 + port) &= ~(  3U << pin);
        *(FL_PINSEL0 + port) |=   func << pin ;
    }
    if (resistor >= 0) {
        *(FL_PINMODE0 + port) &= ~((uint32_t)      3U << pin);
        *(FL_PINMODE0 + port) |=   (uint32_t)resistor << pin ;
    }
}

/* Perhaps move this to i2c.h later??? */

#define FL_I2CPADCFG ((volatile uint32_t *) 0x4002C07C)

#define FL_SDA0_STANDARD_DRIVE  (0<<0)
#define FL_SDA0_FAST_DRIVE      (1<<0)
#define FL_SDA0_GLITCH_ENABLE   (0<<1)
#define FL_SDA0_GLITCH_DISABLE  (1<<1)
#define FL_SCL0_STANDARD_DRIVE  (0<<2)
#define FL_SCL0_FAST_DRIVE      (1<<2)
#define FL_SCL0_GLITCH_ENABLE   (0<<3)
#define FL_SCL0_GLITCH_DISABLE  (1<<3)

/* pins P0.27 and P0.28 */
static inline void fl_i2c_pad_config(const unsigned config) {
    *FL_I2CPADCFG = config;
}

#endif