/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * uart.h - UART driver for communication with host
 *
 */

#ifndef _pato_uart_h_
#define _pato_uart_h_

#define UART_INBUF_SIZE 5
#define UART_OUTBUF_SIZE 5

#ifndef __ASSEMBLY__

extern uint8_t g_uart_outbuf[UART_OUTBUF_SIZE];

extern void uart_init(void);
extern packet_t *uart_recv(void);
extern void uart_send(void);
extern void *uart_outbuf(void);

#endif /* __ASSEMBLY__ */

#endif /* _pato_uart_h_ */
