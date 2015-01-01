/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * config.c - EEPROM-based configuration
 *
 */

#include <string.h>
#include <stdint.h>

#include <avr/eeprom.h>

#include "hd44780.h"

uint32_t EEMEM uart_baudrate = 9600;
uint8_t EEMEM uart_databits = 8;
uint8_t EEMEM uart_parity = 2;
uint8_t EEMEM uart_stopbits = 1;

uint8_t EEMEM hd44780_initfunc = HD44780_CMD_FUNC_SET  |
				 HD44780_CMD_FUNC_2LINES;

