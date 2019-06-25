/* Copyright (C) 2010, 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_I2S_H
#define FASTLIB_I2S_H

#include "fastlib/common.h"

/* important: in mono mode, each word is transmitted twice(!) */

#define FL_I2SDAO       ((volatile uint32_t *) 0x400A8000)
#define FL_I2SDAI       ((volatile uint32_t *) 0x400A8004)
#define FL_I2STXFIFO    ((volatile uint32_t *) 0x400A8008)
#define FL_I2SRXFIFO    ((volatile uint32_t *) 0x400A800C)
#define FL_I2SSTATE     ((volatile uint32_t *) 0x400A8010)
#define FL_I2SDMA1      ((volatile uint32_t *) 0x400A8014)
#define FL_I2SDMA2      ((volatile uint32_t *) 0x400A8018)
#define FL_I2SIRQ       ((volatile uint32_t *) 0x400A801C)
#define FL_I2STXRATE    ((volatile uint32_t *) 0x400A8020)
#define FL_I2SRXRATE    ((volatile uint32_t *) 0x400A8024)
#define FL_I2STXBITRATE ((volatile uint32_t *) 0x400A8028)
#define FL_I2SRXBITRATE ((volatile uint32_t *) 0x400A802C)
#define FL_I2STXMODE    ((volatile uint32_t *) 0x400A8030)
#define FL_I2SRXMODE    ((volatile uint32_t *) 0x400A8034)

static inline void fl_i2s_output_set_config(const unsigned width, const unsigned mono, const unsigned halfperiod,
                                            const unsigned stop, const unsigned reset, const unsigned mute, const unsigned slave) {
    *FL_I2SDAO = width | (mono<<2) | ((halfperiod-1)<<6) | (stop<<3) | (reset<<4) | (mute<<15) | (slave<<5);
}

static inline unsigned fl_i2s_output_get_config(void) {
    return *FL_I2SDAO;
}

static inline void fl_i2s_input_set_config(const unsigned width, const unsigned mono, const unsigned halfperiod,
                                            const unsigned stop, const unsigned reset, const unsigned mute, const unsigned slave) {
    *FL_I2SDAI = width | (mono<<2) | ((halfperiod-1)<<6) | (stop<<3) | (reset<<4) | (mute<<15) | (slave<<5);
}

static inline unsigned fl_i2s_input_get_config(void) {
    return *FL_I2SDAI;
}

static inline void fl_i2s_write_tx_fifo(const unsigned value) {
    *FL_I2STXFIFO = value;
}

static inline unsigned fl_i2s_read_rx_fifo(void) {
    return *FL_I2SRXFIFO;
}

static inline unsigned fl_i2s_status_irq(void) {
    return *FL_I2SSTATE & (1U<<0);
}

static inline unsigned fl_i2s_status_dmareq1(void) {
    return *FL_I2SSTATE & (1U<<1);
}

static inline unsigned fl_i2s_status_dmareq2(void) {
    return *FL_I2SSTATE & (1U<<2);
}

static inline unsigned fl_i2s_get_rx_level(void) {
    return (*FL_I2SSTATE >> 8) & 15;
}

static inline unsigned fl_i2s_get_tx_level(void) {
    return (*FL_I2SSTATE >> 16) & 15;
}

/* rx/tx_state: 0-1     rx/tx-level: 0-15 */
static inline void fl_i2s_config_dma1(const unsigned rx_state, const unsigned tx_state, const unsigned rx_level, const unsigned tx_level) {
    *FL_I2SDMA1 = rx_state | (tx_state<<1) | (rx_level<<8) | (tx_level<<16);
}
static inline void fl_i2s_config_dma2(const unsigned rx_state, const unsigned tx_state, const unsigned rx_level, const unsigned tx_level) {
    *FL_I2SDMA2 = rx_state | (tx_state<<1) | (rx_level<<8) | (tx_level<<16);
}

/* rx/tx_irq: 0-1 */
static inline void fl_i2s_config_irq(const unsigned rx_irq, const unsigned tx_irq, const unsigned rx_level, const unsigned tx_level) {
    *FL_I2SIRQ = rx_irq | (tx_irq<<1) | (rx_level<<8) | (tx_level<<16);
}

/* tx rate = PCLK_I2S * (X/Y) / 2       X and Y: 1-255 (0=no clock) */
static inline void fl_i2s_set_tx_rate(const unsigned X, const unsigned Y) {
    *FL_I2STXRATE = Y | (X<<8);
}
static inline void fl_i2s_set_rx_rate(const unsigned X, const unsigned Y) {
    *FL_I2SRXRATE = Y | (X<<8);
}

/* bitrate: 1-32 */
static inline void fl_i2s_set_tx_bitrate(const unsigned bitrate) {
    *FL_I2STXBITRATE = bitrate-1;
}
static inline void fl_i2s_set_rx_bitrate(const unsigned bitrate) {
    *FL_I2SRXBITRATE = bitrate-1;
}

#define FL_TX_FRACTIONAL_RATE_DIVIDER   0
#define FL_TX_USE_RX_CLOCK              1

static inline void fl_i2s_tx_mode_clock(const unsigned tx_clock) {
    if (tx_clock) *FL_I2STXMODE |=   1U<<1;
    else          *FL_I2STXMODE &= ~(1U<<1);
}

#define FL_RX_FRACTIONAL_RATE_DIVIDER   0
#define FL_RX_USE_TX_CLOCK              1

static inline void fl_i2s_rx_mode_clock(const unsigned rx_clock) {
    if (rx_clock) *FL_I2SRXMODE |=   1U<<1;
    else          *FL_I2SRXMODE &= ~(1U<<1);
}

static inline void fl_i2s_tx_mode_4pin(const unsigned state) {
    if (state) *FL_I2STXMODE |=   1U<<2;
    else       *FL_I2STXMODE &= ~(1U<<2);
}

static inline void fl_i2s_rx_mode_4pin(const unsigned state) {
    if (state) *FL_I2SRXMODE |=   1U<<2;
    else       *FL_I2SRXMODE &= ~(1U<<2);
}

static inline void fl_i2s_tx_clock_output(const unsigned state) {
    if (state) *FL_I2STXMODE |=   1U<<3;
    else       *FL_I2STXMODE &= ~(1U<<3);
}

static inline void fl_i2s_rx_clock_output(const unsigned state) {
    if (state) *FL_I2SRXMODE |=   1U<<3;
    else       *FL_I2SRXMODE &= ~(1U<<3);
}

#endif