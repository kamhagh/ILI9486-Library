/* Copyright (C) 2010, 2011, 2012 by Ivo van Poorten <ivop@euronet.nl>
 * This file is licensed under the terms of the GNU Lesser
 * General Public License, version 3.
 */

#ifndef FASTLIB_UART_H
#define FASTLIB_UART_H

#include "fastlib/common.h"

#define FL_UART_RBR         0x00
#define FL_UART_THR         0x00
#define FL_UART_DLL         0x00

#define FL_UART_DLM         0x04
#define FL_UART_IER         0x04

#define FL_UART_IIR         0x08
#define FL_UART_FCR         0x08

#define FL_UART_LCR         0x0C
#define FL_UART_MCR         0x10    /* UART1 only */
#define FL_UART_LSR         0x14
#define FL_UART_MSR         0x18    /* UART1 only */
#define FL_UART_SCR         0x1C
#define FL_UART_ACR         0x20
#define FL_UART_ICR         0x24    /* UART023 only */
#define FL_UART_FDR         0x28
#define FL_UART_TER         0x30

#define FL_UART_RS485CTRL       0x4C    /* UART1 only */
#define FL_UART_RS485ADRMATCH   0x50    /* UART1 only */
#define FL_UART_RS485DLY        0x54    /* UART1 only */

#define FL_UART0_BASE       0x4000C000
#define FL_UART1_BASE       0x40010000
#define FL_UART2_BASE       0x40098000
#define FL_UART3_BASE       0x4009C000

#define FL_UART(num, field) \
    ((volatile uint32_t *)(fl_uart_num_to_base(num) + FL_UART_##field))

static inline unsigned fl_uart_num_to_base(const unsigned num) {
    switch(num) {
    case 0:  return FL_UART0_BASE;
    case 1:  return FL_UART1_BASE;
    case 2:  return FL_UART2_BASE;
    default: return FL_UART3_BASE;
    }
}

static inline unsigned fl_uart_receive_byte(const unsigned uart) {
    return *FL_UART(uart, RBR);
}

static inline void fl_uart_send_byte(const unsigned uart, const unsigned byte) {
    *FL_UART(uart, THR) = byte;
}

static inline unsigned fl_uart_get_divisor_latch(const unsigned uart) {
    unsigned divisor;
    *FL_UART(uart, LCR) |=   1U<<7;     // enable access to latch

    divisor = *FL_UART(uart, DLL) | (*FL_UART(uart, DLM) << 8);

    *FL_UART(uart, LCR) &= ~(1U<<7);    // disable access to latch
    return divisor;
}

/* divisor: 16-bit value */
static inline void fl_uart_set_divisor_latch(const unsigned uart, const unsigned divisor) {
    *FL_UART(uart, LCR) |=   1U<<7;     // enable access to latch

    *FL_UART(uart, DLL)  =  divisor       & 0xff;
    *FL_UART(uart, DLM)  = (divisor >> 8) & 0xff;

    *FL_UART(uart, LCR) &= ~(1U<<7);    // disable access to latch
}

static inline void fl_uart_rx_data_available_interrupt_enable(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, IER) |=  (1U<<0);
    else       *FL_UART(uart, IER) &= ~(1U<<0);
}

static inline void fl_uart_tx_fifo_empty_interrupt_enable(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, IER) |=  (1U<<1);
    else       *FL_UART(uart, IER) &= ~(1U<<1);
}

static inline void fl_uart_rx_line_status_interrupt_enable(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, IER) |=  (1U<<2);
    else       *FL_UART(uart, IER) &= ~(1U<<2);
}

static inline void fl_uart_end_of_autobaud_interrupt_enable(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, IER) |=  (1U<<8);
    else       *FL_UART(uart, IER) &= ~(1U<<8);
}

static inline void fl_uart_timeout_autobaud_interrupt_enable(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, IER) |=  (1U<<9);
    else       *FL_UART(uart, IER) &= ~(1U<<9);
}

static inline unsigned fl_uart_interrupt_status(const unsigned uart) {
    return !(*FL_UART(uart, IIR) & 1);  // negate, because status is active low
}

#define FL_UART_INTERRUPT_RLS   3   /* rx line status */
#define FL_UART_INTERRUPT_RDA   2   /* rx data available */
#define FL_UART_INTERRUPT_CTI   6   /* character time-out */
#define FL_UART_INTERRUPT_THRE  1   /* tx fifo empty */
#define FL_UART_INTERRUPT_MODEM 0   /* modem interrupt, uart1 only */

static inline unsigned fl_uart_get_interrupt_identification(const unsigned uart) {
    return (*FL_UART(uart, IIR) >> 1) & 7;
}

/* returns 0 or !0 */
static inline unsigned fl_uart_end_of_autobaud_interrupt(const unsigned uart) {
    return *FL_UART(uart, IIR) & (1U<<8);
}
static inline unsigned fl_uart_timeout_autobaud_interrupt(const unsigned uart) {
    return *FL_UART(uart, IIR) & (1U<<9);
}

/* call is more or less mandatory for application (page 305) */
static inline void fl_uart_enable_fifos(const unsigned uart) {
    *FL_UART(uart, FCR) |= 1U<<0;
}
/* disable/enable cycle clears fifo */
static inline void fl_uart_disable_fifos(const unsigned uart) {
    *FL_UART(uart, FCR) &= ~(1U<<0);
}

static inline void fl_uart_reset_rx_fifo(const unsigned uart) {
    *FL_UART(uart, FCR) |= 1U<<1;       // bit is self-clearing
}

static inline void fl_uart_reset_tx_fifo(const unsigned uart) {
    *FL_UART(uart, FCR) |= 1U<<2;       // bit is self-clearing
}

static inline void fl_uart_enable_dma_mode(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, FCR) |=   1U<<3 ;
    else       *FL_UART(uart, FCR) &= ~(1U<<3);
}

static inline void fl_uart_set_rx_trigger_level(const unsigned uart, const unsigned level) {
    if (level != 14) *FL_UART(uart, FCR) &= ~(3U<<6);
    switch(level) {
    default:                               return;
    case 4:  *FL_UART(uart, FCR) |= 1U<<6; return;
    case 8:  *FL_UART(uart, FCR) |= 2U<<6; return;
    case 14: *FL_UART(uart, FCR) |= 3U<<6; return;
    }
}

#define FL_UART_5BIT    0
#define FL_UART_6BIT    1
#define FL_UART_7BIT    2
#define FL_UART_8BIT    3

#define FL_UART_1STOPBIT    0
#define FL_UART_2STOPBITS   1

#define FL_UART_NO_PARITY   0
#define FL_UART_PARITY      1

#define FL_UART_ODD         0
#define FL_UART_EVEN        1
#define FL_UART_FORCE1      2
#define FL_UART_FORCE0      3

static inline void fl_uart_line_config(const unsigned uart, const unsigned wordlength, const unsigned stopbit, \
                                       const unsigned parity_enable, const unsigned parity_type) {
    const unsigned combined = wordlength | (stopbit << 2) | (parity_enable << 3) | (parity_type << 4);
    *FL_UART(uart, LCR) &= ~(0x3fU);
    *FL_UART(uart, LCR) |= combined;
}

static inline void fl_uart_break_transmission(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, LCR) |=   1U<<6 ;
    else       *FL_UART(uart, LCR) &= ~(1U<<6);
}

static inline unsigned fl_uart_get_line_status(const unsigned uart) {
    return *FL_UART(uart, LSR);
}

/* read status first and use functions below to test what is happening */
/* the reason for this is that a status read clears several bits */

#define FL_LSR_TEST(what, where) \
    static inline unsigned fl_uart_##what(const unsigned status) { \
        return status & (where); \
    }

FL_LSR_TEST(rx_fifo_not_empty,      1U<<0);
FL_LSR_TEST(rx_fifo_overun_error,   1U<<1);
FL_LSR_TEST(rx_parity_error,        1U<<2);
FL_LSR_TEST(rx_framing_error,       1U<<3);
FL_LSR_TEST(rx_break_interrupt,     1U<<4);     /* associated with top of fifo */
FL_LSR_TEST(tx_send_register_empty, 1U<<5);
FL_LSR_TEST(tx_empty,               1U<<6);     /* both send and status register */
FL_LSR_TEST(rx_error_entered_fifo,  1U<<7);

static inline void fl_uart_write_scratch_pad(const unsigned uart, const unsigned byte) {
    *FL_UART(uart, SCR) = byte & 0xff;
}

static inline unsigned fl_uart_read_scratch_pad(const unsigned uart) {
    return *FL_UART(uart, SCR);
}

static inline void fl_uart_autobaud_config(const unsigned uart, const unsigned mode, const unsigned autorestart) {
    *FL_UART(uart, ACR) = (mode << 1) | (autorestart << 2);     // writing zeroes to other bits has no impact (p.308/309)
}

static inline void fl_uart_start_autobaud(const unsigned uart) {
    *FL_UART(uart, ACR) |= 1;       // bit is auto-clearing
}

static inline void fl_uart_clear_end_of_autobaud_interrupt(const unsigned uart) {
    *FL_UART(uart, ACR) |= 1U<<8;
}

static inline void fl_uart_clear_autobaud_timeout_interrupt(const unsigned uart) {
    *FL_UART(uart, ACR) |= 1U<<9;
}

static inline void fl_uart_set_fractional_divider(const unsigned uart, const unsigned divaddval, const unsigned mulval) {
    *FL_UART(uart, FDR) &= ~(0xff);
    *FL_UART(uart, FDR) |= divaddval | (mulval<<4);
}

static inline unsigned fl_uart_get_fractional_divider(const unsigned uart) {
    return *FL_UART(uart, FDR);
}

static inline void fl_uart_transmitter_enable(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, TER) |=   1U<<7 ;
    else       *FL_UART(uart, TER) &= ~(1U<<7);
}

// infrared mode only works on uart0, 2 and 3

static inline void fl_uart_infrared_mode(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, ICR) |=   1U<<0;
    else       *FL_UART(uart, ICR) &= ~(1U<<0);
}
static inline void fl_uart_infrared_input_inverted(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, ICR) |=   1U<<1;
    else       *FL_UART(uart, ICR) &= ~(1U<<1);
}
static inline void fl_uart_infrared_fixed_pulsewidth(const unsigned uart, const unsigned state) {
    if (state) *FL_UART(uart, ICR) |=   1U<<2;
    else       *FL_UART(uart, ICR) &= ~(1U<<2);
}
// see UM10360 page 312 for pulsediv values (0-7)
static inline void fl_uart_infrared_config_pulse(const unsigned uart, const unsigned pulsediv) {
    *FL_UART(uart, ICR) &= ~(7U<<3);
    *FL_UART(uart, ICR) |= pulsediv << 3;
}


// functions below only work for uart1 (full modem interface)

static inline void fl_modem_status_interrupt_enable(const unsigned state) {
    if (state) *FL_UART(1, IER) |=  (1U<<3);
    else       *FL_UART(1, IER) &= ~(1U<<3);
}

static inline void fl_modem_cts_interrupt_enable(const unsigned state) {
    if (state) *FL_UART(1, IER) |=  (1U<<7);
    else       *FL_UART(1, IER) &= ~(1U<<7);
}

static inline void fl_modem_control(const unsigned dtr_control, const unsigned rts_control, const unsigned loopback,
                                    const unsigned rts, const unsigned cts) {
    *FL_UART(1, MCR) = dtr_control | (rts_control << 1) | (loopback << 4) | (rts << 6) | (cts << 7);
}

static inline unsigned fl_modem_get_status(void) {
    return *FL_UART(1, MSR);
}

/* read status first and use functions below to test what is happening */
/* the reason for this is that a status read clears several bits */

#define FL_MSR_TEST(what, where) \
    static inline unsigned fl_modem_##what(const unsigned status) { \
        return status & (where); \
    }

FL_MSR_TEST(cts_state_changed, 1U<<0);
FL_MSR_TEST(dsr_state_changed, 1U<<1);
FL_MSR_TEST(low_to_high_ri,    1U<<2);
FL_MSR_TEST(dcd_state_changed, 1U<<3);
FL_MSR_TEST(cts_state,         1U<<4);      /* clear to send */
FL_MSR_TEST(dsr_state,         1U<<5);      /* data set ready */
FL_MSR_TEST(ri_state,          1U<<6);      /* ring indicator */
FL_MSR_TEST(dcd_state,         1U<<7);      /* data carrier detect */

// RS-485/EIA-485 mode functions, also uart1 only

static inline void fl_rs485_normal_multidrop_mode_enable(const unsigned state) {
    if (state) *FL_UART(1, RS485CTRL) |=   1U<<0 ;
    else       *FL_UART(1, RS485CTRL) &= ~(1U<<0);
}

static inline void fl_rs485_receiver_enable(const unsigned state) {
    if (state) *FL_UART(1, RS485CTRL) |=   1U<<1 ;
    else       *FL_UART(1, RS485CTRL) &= ~(1U<<1);
}

static inline void fl_rs485_auto_address_detect_enable(const unsigned state) {
    if (state) *FL_UART(1, RS485CTRL) |=   1U<<2 ;
    else       *FL_UART(1, RS485CTRL) &= ~(1U<<2);
}

static inline void fl_rs485_auto_direction_control_enable(const unsigned state) {
    if (state) *FL_UART(1, RS485CTRL) |=   1U<<4 ;
    else       *FL_UART(1, RS485CTRL) &= ~(1U<<4);
}

static inline void fl_rs485_direction_control_on_rts_pin(void) {
    *FL_UART(1, RS485CTRL) &= ~(1U<<3);
}

static inline void fl_rs485_direction_control_on_dtr_pin(void) {
    *FL_UART(1, RS485CTRL) |= 1U<<3;
}

static inline void fl_rs485_direction_control_polarity(const unsigned oinv) {
    if (oinv) *FL_UART(1, RS485CTRL) |=   1U<<5 ;
    else      *FL_UART(1, RS485CTRL) &= ~(1U<<5);
}

static inline void fl_rs485_set_address_match(const unsigned byte) {
    *FL_UART(1, RS485ADRMATCH) = byte & 0xff;
}

static inline void fl_rs485_set_delay_time(const unsigned byte) {
    *FL_UART(1, RS485DLY) = byte & 0xff;
}

#endif