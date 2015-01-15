/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * config.h - configuration for UART-I2C bridge
 *
 */

#ifndef _pato_bridge_config_h_
#define _pato_bridge_config_h_

#ifdef __AVR_ATmega2560__
#define F_CPU 16000000
#define UART_PORT PORTE
#define UART_DDR  PORTE
#endif
#ifdef __AVR_ATtiny2313__
#define F_CPU 8000000
#define UART_PORT PORTD
#define UART_DDR  PORTD
#endif
#ifdef __AVR_ATmega328P__
#define F_CPU 8000000
#define UART_PORT PORTD
#define UART_DDR  PORTD
#endif

#endif /* _pato_bridge_config_h_ */
