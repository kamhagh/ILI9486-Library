/* Copyright (C) 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_SSP_H
#define FASTLIB_SSP_H

#include "fastlib/common.h"

#define FL_SSP0         ((volatile uint32_t *) 0x40088000)
#define FL_SSP1         ((volatile uint32_t *) 0x40030000)

#define FL_SSP_CR0      0x00
#define FL_SSP_CR1      0x04
#define FL_SSP_DR       0x08
#define FL_SSP_SR       0x0C
#define FL_SSP_CPSR     0x10
#define FL_SSP_IMSC     0x14
#define FL_SSP_RIS      0x18
#define FL_SSP_MIS      0x1C
#define FL_SSP_ICR      0x20
#define FL_SSP_DMACR    0x24

#define FL_SSP(num, reg)    (FL_SSP##num + (volatile unit32_t *)FL_SSP_##reg)

// NOT FINISHED YET

#endif