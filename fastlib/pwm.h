/* Copyright (C) 2010, 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_PWM_H
#define FASTLIB_PWM_H

#include "fastlib/common.h"

#define FL_PWM1IR   ((volatile uint32_t *) 0x40018000)

#define FL_PWM1TCR  ((volatile uint32_t *) 0x40018004)
#define FL_PWM1TC   ((volatile uint32_t *) 0x40018008)

#define FL_PWM1PR   ((volatile uint32_t *) 0x4001800C)
#define FL_PWM1PC   ((volatile uint32_t *) 0x40018010)

#define FL_PWM1MCR  ((volatile uint32_t *) 0x40018014)
#define FL_PWM1MR0  ((volatile uint32_t *) 0x40018018)
#define FL_PWM1MR1  ((volatile uint32_t *) 0x4001801C)
#define FL_PWM1MR2  ((volatile uint32_t *) 0x40018020)
#define FL_PWM1MR3  ((volatile uint32_t *) 0x40018024)
#define FL_PWM1MR4  ((volatile uint32_t *) 0x40018040)
#define FL_PWM1MR5  ((volatile uint32_t *) 0x40018044)
#define FL_PWM1MR6  ((volatile uint32_t *) 0x40018048)

#define FL_PWM1CCR  ((volatile uint32_t *) 0x40018028)
#define FL_PWM1CR0  ((volatile uint32_t *) 0x4001802C)
#define FL_PWM1CR1  ((volatile uint32_t *) 0x40018030)
#define FL_PWM1CR2  ((volatile uint32_t *) 0x40018034)
#define FL_PWM1CR3  ((volatile uint32_t *) 0x40018038)

#define FL_PWM1PCR  ((volatile uint32_t *) 0x4001804C)
#define FL_PWM1LER  ((volatile uint32_t *) 0x40018050)
#define FL_PWM1CTCR ((volatile uint32_t *) 0x40018070)

/* match channel: 0-8 (0-6 pwm, 7-8 cap0-1)
 * returns 0 or !0
 */
static inline int fl_pwm_interrupt_status(const unsigned channel) {
    int remap;
         if (channel >= 7) remap = -3;
    else if (channel >= 4) remap =  4;
    return *FL_PWM1IR & (1U << (channel+remap));
}

static inline void fl_pwm_interrupt_clear(const unsigned channel) {
    int remap;
         if (channel >= 7) remap = -3;
    else if (channel >= 4) remap =  4;
    *FL_PWM1IR = 1U << (channel+remap);
}

static inline void fl_pwm_timer_counter_enable(const unsigned state) {
    if (state)  *FL_PWM1TCR |=  1;
    else        *FL_PWM1TCR &= ~1U;
}

static inline void fl_pwm_timer_counter_reset(const unsigned state) {
    if (state)  *FL_PWM1TCR |=  2;
    else        *FL_PWM1TCR &= ~2U;
}

static inline void fl_pwm_enable(const unsigned state) {
    if (state)  *FL_PWM1TCR |=  8;
    else        *FL_PWM1TCR &= ~8U;
}

static inline unsigned fl_pwm_get_timer_counter(void) {
    return *FL_PWM1TC;
}

static inline void fl_pwm_set_timer_counter(unsigned value) {
    *FL_PWM1TC = value;
}

/* value: 1-... */
static inline void fl_pwm_set_prescale(const unsigned value) {
    *FL_PWM1PR = value - 1;
}

static inline unsigned fl_pwm_get_prescale_counter(void) {
    return *FL_PWM1PR + 1;
}

static inline void fl_pwm_set_timer_mode(void) {
    *FL_PWM1CTCR &= ~3U;
}

/* mode: 1-3 (FL_RISE/FALL/BOTH)    pcap1: 0-1 */
static inline void fl_pwm_set_counter_mode(const unsigned mode, const unsigned pcap1n) {
    *FL_PWM1CTCR = mode | (pcap1n << 2);
}

/* channel: 0-6 */
static inline void fl_pwm_config_match(const unsigned channel, const unsigned interrupt, const unsigned reset, const unsigned stop) {
    const unsigned mask = (interrupt | (reset << 1) | (stop << 2)) << ((channel<<1)+channel);
    *FL_PWM1MCR &= ~mask;
    *FL_PWM1MCR |=  mask;
}

/* channel: 0-6 */
static inline void fl_pwm_set_match(const unsigned channel, const unsigned value) {
    if (channel <= 3) *(FL_PWM1MR0+channel)      = value;
    else              *(FL_PWM1MR0+channel+0x18) = value;
}

/* pcap1n: 0-1 */
static inline void fl_pwm_config_capture(const unsigned pcap1n, const unsigned rise, const unsigned fall, const unsigned interrupt) {
    const unsigned mask = (rise | (fall << 1) | (interrupt << 2)) << ((pcap1n<<1)+pcap1n);
    *FL_PWM1CCR &= ~mask;
    *FL_PWM1CCR |=  mask;
}

/* pcap1n: 0-1 (2,3?) */
static inline unsigned fl_pwm_get_capture(const unsigned pcap1n) {
    return *(FL_PWM1CR0+pcap1n);
}

/* channel: 2-6  edges: 0-1 (single/double) */
static inline void fl_pwm_config_edges(const unsigned channel, const unsigned edges) {
    if (edges) *FL_PWM1PCR |=   edges << channel ;
    else       *FL_PWM1PCR &= ~(edges << channel);
}

/* channel: 1-6     state: 0-1 */
static inline void fl_pwm_output_enable(const unsigned channel, const unsigned state) {
    if (state) *FL_PWM1PCR |=   1U << (channel+8) ;
    else       *FL_PWM1PCR &= ~(1U << (channel+8));
}

static inline void fl_pwm_latch_match_channel(const unsigned channel) {
    *FL_PWM1LER |= 1U << channel;
}

static inline void fl_pwm_latch_match_mask(const unsigned mask) {
    *FL_PWM1LER = mask;
}

static inline void fl_pwm_latch_match_all(void) {
    fl_pwm_latch_match_mask(0x7f);
}

#endif