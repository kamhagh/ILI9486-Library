/* Copyright (C) 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_SYSTICK_H
#define FASTLIB_SYSTICK_H

#include "fastlib/common.h"

#define FL_STCTRL       ((volatile uint32_t *) 0xE000E010)
#define FL_STRELOAD     ((volatile uint32_t *) 0xE000E014)
#define FL_STCURR       ((volatile uint32_t *) 0xE000E018)
#define FL_STCALIB      ((volatile uint32_t *) 0xE000E01C)

static inline void fl_systick_control(const unsigned enable,
                                      const unsigned interrupt_enable,
                                      const unsigned use_cpu_clock) {
    *FL_STCTRL = enable | (interrupt_enable<<1) | (use_cpu_clock<<2);
}

static inline unsigned fl_systick_get_counter_flag(void) {
    return *FL_STCTRL & (1U<<16);
}

static inline void fl_systick_set_reload_value(const unsigned value) {
    *FL_STRELOAD = value;
}

static inline unsigned fl_systick_get_reload_value(void) {
    return *FL_STRELOAD;
}

static inline unsigned fl_systick_get_current_value(void) {
    return *FL_STCURR;
}

static inline void fl_systick_set_calibration(const unsigned reload_value,
                                              const unsigned has_skew,
                                              const unsigned no_ext_ref_clock) {
    *FL_STCALIB = reload_value | (has_skew<<30) | (no_ext_ref_clock<<31);
}

static inline unsigned fl_systick_get_calibration_reload_value(void) {
    return *FL_STCALIB & ((1U<<24)-1);
}

static inline unsigned fl_systick_get_calibration_skew(void) {
    return *FL_STCALIB & (1U<<30);
}

static inline unsigned fl_systick_get_calibration_noref(void) {
    return *FL_STCALIB & (1U<<31);
}

#endif