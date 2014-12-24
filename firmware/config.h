/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * config.h - EEPROM-based configuration
 *
 */

#ifndef _pato_config_h_
#define _pato_config_h_

extern uint32_t EEMEM uart_baudrate;
extern uint8_t EEMEM uart_databits;
extern uint8_t EEMEM uart_parity;
extern uint8_t EEMEM uart_stopbits;

#endif /* _pato_config_h_ */
