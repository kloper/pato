/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * config.h - EEPROM-based configuration
 *
 */

#ifndef _pato_config_h_
#define _pato_config_h_

#ifdef __AVR_ATmega2560__
#define F_CPU 16000000
#endif
#ifdef __AVR_ATtiny2313__
#define F_CPU 8000000
#endif

#undef HAVE_UART 
#define HAVE_TWI

#ifndef __ASSEMBLY__

#if defined(HAVE_UART)
extern uint32_t EEMEM uart_baudrate;
extern uint8_t EEMEM uart_databits;
extern uint8_t EEMEM uart_parity;
extern uint8_t EEMEM uart_stopbits;
#endif /* HAVE_UART */

extern uint8_t EEMEM hd44780_initfunc;

extern uint8_t EEMEM tty_height;
extern uint8_t EEMEM tty_linemap[8];
extern uint8_t EEMEM tty_policy;

#if defined (HAVE_TWI)
extern uint32_t EEMEM twi_baudrate;
extern uint8_t EEMEM twi_slaveaddr;
#endif /* HAVE_TWI */

#endif /* __ASSEMBLY__ */
#endif /* _pato_config_h_ */
