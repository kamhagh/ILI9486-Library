/* Copyright (C) 2010, 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_COMMON_H
#define FASTLIB_COMMON_H

#include <stdint.h>

#define FL_IGNORE (-1)

#define FL_OFF      0
#define FL_ON       1

#define FL_INPUT    0
#define FL_OUTPUT   1

#define FL_RISE     0
#define FL_FALL     1
#define FL_BOTH     2

#define FL_DISABLE  0
#define FL_ENABLE   1

#define FL_DISCONNECT   0
#define FL_CONNECT      1

#define FL_SINGLE_EDGE  0
#define FL_DOUBLE_EDGE  1

#define FL_8BITS    0
#define FL_16BITS   1
#define FL_32BITS   3

#define FL_STEREO   0
#define FL_MONO     1

#define FL_MASTER   0
#define FL_SLAVE    1

#define FL_LITTLE_ENDIAN    0
#define FL_BIG_ENDIAN       1

#endif