/* Copyright (C) 2010, 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_GPIO_H
#define FASTLIB_GPIO_H

#include "fastlib/common.h"

static inline volatile uint32_t *fl_gpio_get_base(const int port) {
    switch (port) {
    case 0:  return (volatile uint32_t *) 0x2009c000;
    case 1:  return (volatile uint32_t *) 0x2009c020;
    case 2:  return (volatile uint32_t *) 0x2009c040;
    case 3:  return (volatile uint32_t *) 0x2009c060;
    case 4:  return (volatile uint32_t *) 0x2009c080;
    default: return (volatile uint32_t *) 0;
    }
}

#define FL_GPIO_OVERALL_INT_STATUS      ((volatile uint32_t *) 0x40028080)

#define FL_GPIO_PORT0_RISE_INT_STATUS   ((volatile uint32_t *) 0x40028084)
#define FL_GPIO_PORT0_FALL_INT_STATUS   ((volatile uint32_t *) 0x40028088)
#define FL_GPIO_PORT0_INT_CLEAR         ((volatile uint32_t *) 0x4002808C)
#define FL_GPIO_PORT0_RISE_INT          ((volatile uint32_t *) 0x40028090)
#define FL_GPIO_PORT0_FALL_INT          ((volatile uint32_t *) 0x40028094)

#define FL_GPIO_PORT2_RISE_INT_STATUS   ((volatile uint32_t *) 0x400280A4)
#define FL_GPIO_PORT2_FALL_INT_STATUS   ((volatile uint32_t *) 0x400280A8)
#define FL_GPIO_PORT2_INT_CLEAR         ((volatile uint32_t *) 0x400280AC)
#define FL_GPIO_PORT2_RISE_INT          ((volatile uint32_t *) 0x400280B0)
#define FL_GPIO_PORT2_FALL_INT          ((volatile uint32_t *) 0x400280B4)

/* dir: 0=input, 1=output */
static inline void fl_gpio_set_direction(const unsigned port, const unsigned int bitmask, const unsigned dir) {
    if (dir) *(fl_gpio_get_base(port)) |=  bitmask;
    else     *(fl_gpio_get_base(port)) &= ~bitmask;
}

static inline void fl_gpio_set_value(const unsigned port, const unsigned bitmask) {
    *(fl_gpio_get_base(port) + 6) = bitmask;
}

static inline void fl_gpio_clear_value(const unsigned port, const unsigned bitmask) {
    *(fl_gpio_get_base(port) + 7) = bitmask;
}

static inline uint32_t fl_gpio_get_value(const unsigned port, const unsigned bitmask) {
    return *(fl_gpio_get_base(port) + 5);
}

static inline void fl_gpio_set_mask(const unsigned port, const unsigned bitmask) {
    *(fl_gpio_get_base(port) + 4) |= bitmask;
}

static inline void fl_gpio_clear_mask(const unsigned port, const unsigned bitmask) {
    *(fl_gpio_get_base(port) + 4) &= ~bitmask;
}

/* interrupts only on port 0 or 2 */

/* return: 0 = none, !0 = at least one */
static inline int fl_gpio_port_interrupt_status(const unsigned port) {
    return *FL_GPIO_OVERALL_INT_STATUS & (1U<<port);
}

static inline void fl_gpio_interrupt_enable(const unsigned port, const unsigned bitmask, const unsigned edge) {
    if (port == 0) {
        if (edge == FL_RISE) *FL_GPIO_PORT0_RISE_INT |= bitmask;
        else                 *FL_GPIO_PORT0_FALL_INT |= bitmask;
    } else {
        if (edge == FL_RISE) *FL_GPIO_PORT2_RISE_INT |= bitmask;
        else                 *FL_GPIO_PORT2_FALL_INT |= bitmask;
    }
}

static inline void fl_gpio_interrupt_disable(const unsigned port, const unsigned bitmask, const unsigned edge) {
    if (port == 0) {
        if (edge == FL_RISE) *FL_GPIO_PORT0_RISE_INT &= ~bitmask;
        else                 *FL_GPIO_PORT0_FALL_INT &= ~bitmask;
    } else {
        if (edge == FL_RISE) *FL_GPIO_PORT2_RISE_INT &= ~bitmask;
        else                 *FL_GPIO_PORT2_FALL_INT &= ~bitmask;
    }
}

static inline uint32_t fl_gpio_interrupt_status(const unsigned port, const unsigned bitmask, const unsigned edge) {
    if (port == 0) {
        if (edge == FL_RISE) return *FL_GPIO_PORT0_RISE_INT_STATUS & bitmask;
        else                 return *FL_GPIO_PORT0_FALL_INT_STATUS & bitmask;
    } else {
        if (edge == FL_RISE) return *FL_GPIO_PORT2_RISE_INT_STATUS & bitmask;
        else                 return *FL_GPIO_PORT2_FALL_INT_STATUS & bitmask;
    }
}

static inline void fl_gpio_interrupt_clear(const unsigned port, const unsigned bitmask) {
    if (port == 0) *FL_GPIO_PORT0_INT_CLEAR = bitmask;
    else           *FL_GPIO_PORT2_INT_CLEAR = bitmask;
}

#endif