/* Copyright (C) 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_NVIC_H
#define FASTLIB_NVIC_H

#include "fastlib/common.h"

#define FL_NVIC_ISER0           ((volatile uint32_t *) 0xE000E100)
#define FL_NVIC_ISER1           ((volatile uint32_t *) 0xE000E104)
#define FL_NVIC_ICER0           ((volatile uint32_t *) 0xE000E180)
#define FL_NVIC_ICER1           ((volatile uint32_t *) 0xE000E184)
#define FL_NVIC_ISPR0           ((volatile uint32_t *) 0xE000E200)
#define FL_NVIC_ISPR1           ((volatile uint32_t *) 0xE000E204)
#define FL_NVIC_ICPR0           ((volatile uint32_t *) 0xE000E280)
#define FL_NVIC_ICPR1           ((volatile uint32_t *) 0xE000E284)
#define FL_NVIC_IABR0           ((volatile uint32_t *) 0xE000E300)
#define FL_NVIC_IABR1           ((volatile uint32_t *) 0xE000E304)
#define FL_NVIC_IPR0            ((volatile uint32_t *) 0xE000E400)
#define FL_NVIC_IPR1            ((volatile uint32_t *) 0xE000E404)
#define FL_NVIC_IPR2            ((volatile uint32_t *) 0xE000E408)
#define FL_NVIC_IPR3            ((volatile uint32_t *) 0xE000E40C)
#define FL_NVIC_IPR4            ((volatile uint32_t *) 0xE000E410)
#define FL_NVIC_IPR5            ((volatile uint32_t *) 0xE000E414)
#define FL_NVIC_IPR6            ((volatile uint32_t *) 0xE000E418)
#define FL_NVIC_IPR7            ((volatile uint32_t *) 0xE000E41C)
#define FL_NVIC_IPR8            ((volatile uint32_t *) 0xE000E420)
#define FL_NVIC_STIR            ((volatile uint32_t *) 0xE000EF00)

#define FL_NVIC_INT_WDT         0
#define FL_NVIC_INT_TIMER0      1
#define FL_NVIC_INT_TIMER1      2
#define FL_NVIC_INT_TIMER2      3
#define FL_NVIC_INT_TIMER3      4
#define FL_NVIC_INT_UART0       5
#define FL_NVIC_INT_UART1       6
#define FL_NVIC_INT_UART2       7
#define FL_NVIC_INT_UART3       8
#define FL_NVIC_INT_PWM         9
#define FL_NVIC_INT_I2C0        10
#define FL_NVIC_INT_I2C1        11
#define FL_NVIC_INT_I2C2        12
#define FL_NVIC_INT_SPI         13
#define FL_NVIC_INT_SSP0        14
#define FL_NVIC_INT_SSP1        15
#define FL_NVIC_INT_PLL0        16
#define FL_NVIC_INT_RTC         17
#define FL_NVIC_INT_EINT0       18
#define FL_NVIC_INT_EINT1       19
#define FL_NVIC_INT_EINT2       20
#define FL_NVIC_INT_EINT3       21
#define FL_NVIC_INT_ADC         22
#define FL_NVIC_INT_BOD         23
#define FL_NVIC_INT_USB         24
#define FL_NVIC_INT_CAN         25
#define FL_NVIC_INT_DMA         26
#define FL_NVIC_INT_I2S         27
#define FL_NVIC_INT_ENET        28
#define FL_NVIC_INT_RIT         29
#define FL_NVIC_INT_MCPWM       30
#define FL_NVIC_INT_QEI         31

#define FL_NVIC_INT_PLL1        32
#define FL_NVIC_INT_USBACT      33
#define FL_NVIC_INT_CANACT      34

#define FL_NVIC_INTERRUPT_FUNC(x, y) \
    static inline void fl_nvic_interrupt_##x(const unsigned interrupt) { \
        if (interrupt < 32) *FL_NVIC_##y##0 |= 1U<<interrupt;            \
        else                *FL_NVIC_##y##1 |= 1U<<(interrupt-32);       \
    } \
    static inline unsigned fl_nvic_interrupt_##x##_status(const unsigned interrupt) { \
        if (interrupt < 32) return (*FL_NVIC_##y##0) & (1U<<interrupt);               \
        else                return (*FL_NVIC_##y##1) & (1U<<(interrupt-32));          \
    }

FL_NVIC_INTERRUPT_FUNC(set_enable,    ISER)
FL_NVIC_INTERRUPT_FUNC(clear_enable,  ICER)
FL_NVIC_INTERRUPT_FUNC(set_pending,   ISPR)
FL_NVIC_INTERRUPT_FUNC(clear_pending, ICPR)

static inline unsigned fl_nvic_interrupt_active_bit_status(const unsigned interrupt) {
    if (interrupt < 32) return (*FL_NVIC_IABR0) & (1U<<interrupt);
    else                return (*FL_NVIC_IABR1) & (1U<<(interrupt-32));
}

static inline void fl_nvic_interrupt_set_priority(const unsigned interrupt, const unsigned priority) {
    const unsigned i = interrupt >> 3,
                   j = (31      <<3) << ((interrupt&3)<<3),
                   k = (priority<<3) << ((interrupt&3)<<3);
    *(FL_NVIC_IPR0 + i) &= ~j;
    *(FL_NVIC_IPR0 + i) |=  k;
}

static inline void fl_nvic_interrupt_software_trigger(const unsigned interrupt) {
    *FL_NVIC_STIR = interrupt;
}

#endif