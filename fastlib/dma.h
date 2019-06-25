/* Copyright (C) 2010, 2011 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_DMA_H
#define FASTLIB_DMA_H

#include "fastlib/common.h"

#define FL_DMACIntStat          ((volatile uint32_t *) 0x50004000)
#define FL_DMACIntTCStat        ((volatile uint32_t *) 0x50004004)
#define FL_DMACIntTCClear       ((volatile uint32_t *) 0x50004008)
#define FL_DMACIntErrStat       ((volatile uint32_t *) 0x5000400C)
#define FL_DMACIntErrClr        ((volatile uint32_t *) 0x50004010)
#define FL_DMACRawIntTCStat     ((volatile uint32_t *) 0x50004014)
#define FL_DMACRawIntErrStat    ((volatile uint32_t *) 0x50004018)
#define FL_DMACEnbldChns        ((volatile uint32_t *) 0x5000401C)
#define FL_DMACSoftBReq         ((volatile uint32_t *) 0x50004020)
#define FL_DMACSoftSReq         ((volatile uint32_t *) 0x50004024)
#define FL_DMACSoftLBReq        ((volatile uint32_t *) 0x50004028)
#define FL_DMACSoftLSReq        ((volatile uint32_t *) 0x5000402C)
#define FL_DMACConfig           ((volatile uint32_t *) 0x50004030)
#define FL_DMACSync             ((volatile uint32_t *) 0x50004034)
#define FL_DMAReqSel            ((volatile uint32_t *) 0x400FC1C4)

#define FL_DMACC0   0x50004100
#define FL_DMACC1   0x50004120
#define FL_DMACC2   0x50004140
#define FL_DMACC3   0x50004160
#define FL_DMACC4   0x50004180
#define FL_DMACC5   0x500041A0
#define FL_DMACC6   0x500041C0
#define FL_DMACC7   0x500041E0

#define FL_DMA_SrcAddr  0x00
#define FL_DMA_DestAddr 0x04
#define FL_DMA_LLI      0x08
#define FL_DMA_Control  0x0C
#define FL_DMA_Config   0x10

#define FL_DMA(channel, field) \
    ((volatile uint32_t *)(fl_dma_channel_to_base(channel) + FL_DMA_##field))

static inline unsigned fl_dma_channel_to_base(const unsigned channel) {
    switch(channel) {
    case 0:  return FL_DMACC0;
    case 1:  return FL_DMACC1;
    case 2:  return FL_DMACC2;
    case 3:  return FL_DMACC3;
    case 4:  return FL_DMACC4;
    case 5:  return FL_DMACC5;
    case 6:  return FL_DMACC6;
    default: return FL_DMACC7;
    }
}

// channel: 0-7
static inline unsigned fl_dma_channel_has_active_interrupt_request(const unsigned channel) {
    return *FL_DMACIntStat & (1U<<channel);
}

static inline unsigned fl_dma_channel_has_active_terminal_count_interrupt_request(const unsigned channel) {
    return *FL_DMACIntTCStat & (1U<<channel);
}
static inline void fl_dma_clear_terminal_count_interrupt_request(const unsigned channel) {
    *FL_DMACIntTCClear |= 1U<<channel;
}
static inline unsigned fl_dma_channel_has_active_terminal_count_interrupt_request_raw(const unsigned channel) {
    return *FL_DMACRawIntTCStat & (1U<<channel);     // raw is prior to masking
}

static inline unsigned fl_dma_channel_has_active_error_interrupt_request(const unsigned channel) {
    return *FL_DMACIntErrStat & (1U<<channel);
}
static inline void fl_dma_clear_error_interrupt_request(const unsigned channel) {
    *FL_DMACIntErrClr |= 1U<<channel;
}
static inline unsigned fl_dma_channel_has_active_error_interrupt_request_raw(const unsigned channel) {
    return *FL_DMACRawIntErrStat & (1U<<channel);
}

static inline unsigned fl_dma_channel_is_enabled(const unsigned channel) {
    return *FL_DMACEnbldChns & (1U<<channel);
}

static inline void fl_dma_generate_software_burst_request(const unsigned request) {
    *FL_DMACSoftBReq |= 1U << request;
}
static inline unsigned fl_dma_software_burst_request_sources(void) {
    return *FL_DMACSoftBReq;
}

static inline void fl_dma_generate_software_single_request(const unsigned request) {
    *FL_DMACSoftSReq |= 1U << request;
}
static inline unsigned fl_dma_software_single_request_sources(void) {
    return *FL_DMACSoftSReq;
}

static inline void fl_dma_generate_software_last_burst_request(const unsigned request) {
    *FL_DMACSoftLBReq |= 1U << request;
}
static inline unsigned fl_dma_software_last_burst_request_sources(void) {
    return *FL_DMACSoftLBReq;
}

static inline void fl_dma_generate_software_last_single_request(const unsigned request) {
    *FL_DMACSoftLSReq |= 1U << request;
}
static inline unsigned fl_dma_software_last_single_request_sources(void) {
    return *FL_DMACSoftLSReq;
}

static inline void fl_dma_enable(const unsigned state) {
    if (state) *FL_DMACConfig |=   1U<<0 ;
    else       *FL_DMACConfig &= ~(1U<<0);
}

static inline unsigned fl_dma_is_enabled(void) {
    return *FL_DMACConfig & 1;
}

static inline void fl_dma_set_endianness(const unsigned endianness) {
    if (endianness) *FL_DMACConfig |=   1U<<1 ;
    else            *FL_DMACConfig &= ~(1U<<1);
}

static inline void fl_dma_request_synchronization_enable(const unsigned request, const unsigned state) {
    if (!state) *FL_DMACSync |=   1U<<request ;     // note that 0 is enabled
    else        *FL_DMACSync &= ~(1U<<request);
}

#define FL_DMA_UART     0
#define FL_DMA_TIMER    1

// select between uart or timer dma requests for inputs 8 through 15
// input: 8-16  which: 0-1
static inline void fl_dma_request_select(const unsigned input, const unsigned which) {
    if (which) *FL_DMAReqSel |=   1U<<(which-8) ;
    else       *FL_DMAReqSel &= ~(1U<<(which-8));
}

// channel: 0-7
static inline void fl_dma_set_srcaddr(const unsigned channel, const void *srcaddr) {
    *FL_DMA(channel, SrcAddr) = (unsigned) srcaddr;
}

static inline void fl_dma_set_destaddr(const unsigned channel, const void *destaddr) {
    *FL_DMA(channel, DestAddr) = (unsigned) destaddr;
}

// lli must be word-aligned
static inline void fl_dma_set_next_lli(const unsigned channel, const void *lli) {
    *FL_DMA(channel, LLI) = (unsigned) lli & (~3U);
}

#define FL_NO_SRC_INCREMENT     0
#define FL_SRC_INCREMENT        1
#define FL_NO_DEST_INCREMENT    0
#define FL_DEST_INCREMENT       1

static inline unsigned fl_dma_size_to_mask(const unsigned size) {
    switch(size) {
    case 4:     return 1;
    case 8:     return 2;
    case 16:    return 3;
    case 32:    return 4;
    case 64:    return 5;
    case 128:   return 6;
    case 256:   return 7;
    default:    return 0;
    }
}

static inline unsigned fl_dma_width_to_mask(const unsigned width) {
    switch(width) {
    case 16:    return 1;
    case 32:    return 2;
    default:    return 0;
    }
}

// transfer_size: 12 bits
// size: 1, 4, 8, 16, 32, 64, 128 or 256
// width: 8, 16 or 32
// irq: 0-1
static inline void fl_dma_channel_control(const unsigned channel, const unsigned transfer_size,
                                          const unsigned src_burst_size, const unsigned dest_burst_size,
                                          const unsigned src_width, const unsigned dest_width,
                                          const unsigned src_increment, const unsigned dest_increment,
                                          const unsigned terminal_count_irq) {
    *FL_DMA(channel, Control) = transfer_size & 0xfff
                              | (fl_dma_size_to_mask(src_burst_size)  << 12)
                              | (fl_dma_size_to_mask(dest_burst_size) << 15)
                              | (fl_dma_width_to_mask(src_width)  << 18)
                              | (fl_dma_width_to_mask(dest_width) << 21)
                              | (src_increment  << 26)
                              | (dest_increment << 27)
                              | (terminal_count_irq << 31);
}

static inline void fl_dma_channel_control_by_mask(const unsigned channel, const unsigned mask) {
    *FL_DMA(channel, Control) = mask;
}

static inline unsigned fl_dma_channel_get_control_mask(const unsigned channel) {
    return *FL_DMA(channel, Control);
}

static inline void fl_dma_channel_terminal_count_irq_enable(const unsigned channel, const unsigned state) {
    if (state) *FL_DMA(channel, Control) |=   1U<<31 ;
    else       *FL_DMA(channel, Control) &= ~(1U<<31);
}

// channel configuration constants, src and dest peripherals
#define FL_DMA_SINGLE_REQUEST_SSP0_Tx   0
#define FL_DMA_SINGLE_REQUEST_SSP0_Rx   1
#define FL_DMA_SINGLE_REQUEST_SSP1_Tx   2
#define FL_DMA_SINGLE_REQUEST_SSP1_Rx   3
#define FL_DMA_SINGLE_REQUEST_ADC       4

#define FL_DMA_BURST_REQUEST_SSP0_Tx    0
#define FL_DMA_BURST_REQUEST_SSP0_Rx    1
#define FL_DMA_BURST_REQUEST_SSP1_Tx    2
#define FL_DMA_BURST_REQUEST_SSP1_Rx    3
#define FL_DMA_BURST_REQUEST_ADC        4
#define FL_DMA_BURST_REQUEST_I2S_CH0    5
#define FL_DMA_BURST_REQUEST_I2S_CH1    6
#define FL_DMA_BURST_REQUEST_DAC        7
#define FL_DMA_BURST_REQUEST_UART0_Tx   8
#define FL_DMA_BURST_REQUEST_UART0_Rx   9
#define FL_DMA_BURST_REQUEST_UART1_Tx   10
#define FL_DMA_BURST_REQUEST_uART1_Rx   11
#define FL_DMA_BURST_REQUEST_UART2_Tx   12
#define FL_DMA_BURST_REQUEST_UART2_Rx   13
#define FL_DMA_BURST_REQUEST_UART3_Tx   14
#define FL_DMA_BURST_REQUEST_UART3_Rx   15

#define FL_DMA_PERIPHERAL_IS_MEMORY     0

// transfer types
#define FL_DMA_MEMORY_TO_MEMORY         0
#define FL_DMA_MEMORY_TO_PERIPHERAL     1
#define FL_DMA_PERIPHERAL_TO_MEMORY     2
#define FL_DMA_PERIPHERAL_TO_PERIPHERAL 3

static inline void fl_dma_channel_config(const unsigned channel,
    const unsigned enable, const unsigned src_peripheral, const unsigned dest_peripheral,
    const unsigned transfer_type, const unsigned mask_error, const unsigned mask_tc) {
    *FL_DMA(channel, Config) = enable | (src_peripheral<<1) | (dest_peripheral<<6) |
                               (transfer_type<<11) | (mask_error<<14) | (mask_tc<<15);
}

// return 0 or !0 (channel fifo has data)
static inline unsigned fl_dma_channel_is_active(const unsigned channel) {
    return *FL_DMA(channel, Config) & (1U<<17);
}

static inline void fl_dma_channel_halt(const unsigned channel, const unsigned state) {
    if (state) *FL_DMA(channel, Config) |=   1U<<18 ;
    else       *FL_DMA(channel, Config) &= ~(1U<<18);
}

#endif