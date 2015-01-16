/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * uart.h - UART side of the UART-I2C bridge
 *
 */

#ifndef _pato_bridge_uart_h_
#define _pato_bridge_uart_h_

#define UART_INBUF_SIZE 256
#define UART_OUTBUF_SIZE 256

#define UART_BAUD_RATE 57600
#define UART_PARITY 2
#define UART_STOPBITS 1
#define UART_DATABITS 8

#define UART_UBRR (F_CPU/16/UART_BAUD_RATE-1)
#define UART_APPROX_BAUD_RATE (F_CPU/16/(UART_UBRR+1))

extern void uart_init(void);
extern uint8_t uart_recv(void);
extern void uart_continue(void);
extern void *uart_outbuf(void);
extern void *uart_inbuf(void);
extern void uart_send(void);

#endif /* _pato_bridge_uart_h_ */
