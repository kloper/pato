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
void uart_init(void);
packet_t *uart_recv(void);
#endif /* __ASSEMBLY__ */

#endif /* _pato_uart_h_ */
