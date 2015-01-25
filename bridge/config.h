/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * config.h - configuration for UART-I2C bridge
 *
 */

#ifndef _pato_bridge_config_h_
#define _pato_bridge_config_h_

#if defined(__AVR_ATmega2560__)
#define F_CPU 16000000
#define UART_PORT PORTE
#define UART_DDR  PORTE
#endif
#if defined(__AVR_ATmega328P__)
#define F_CPU 16000000
#define UART_PORT PORTD
#define UART_DDR  PORTD
#endif
#if defined(__AVR_ATmega328__)
#define F_CPU 3686400
#define UART_PORT PORTD
#define UART_DDR  PORTD
#endif

#define WDT_PRESCALE 0x5
#define WDT_RETRIES 4

#endif /* _pato_bridge_config_h_ */
