/** -*- C -*-
 * \file
 *
 * \brief EEPROM-based configuration
 *
 * Copyright (c) 2014-2015 Dimitry Kloper <kloper@users.sf.net>. 
 * All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * official policies, either expressed or implied, of the Pato Project.
 */

#include <string.h>
#include <stdint.h>

#include <avr/eeprom.h>

#include "config.h"
#include "hd44780.h"

pato_config_t EEMEM g_pato_config = {
   /* uint8_t hd44780_initfunc */ HD44780_CMD_FUNC_SET |
                                  HD44780_CMD_FUNC_2LINES,

   /* uint32_t uart_baudrate */ 9600,
   /* uint8_t uart_databits */ 8,
   /* uint8_t uart_parity */ 2,
   /* uint8_t uart_stopbits */ 1,

#if defined(DISPLAY2040)   
   /* uint8_t tty_height */ 4,   
   /* uint8_t tty_linemap[8] */ { 0, 20, 64, 84, 20, 40, 84, 104 },
   /* uint8_t tty_policy */ (4<<1),   
#endif
#if defined(DISPLAY1602)   
   /* uint8_t tty_height */ 2,   
   /* uint8_t tty_linemap[8] */ { 0, 16, 40, 56 },
   /* uint8_t tty_policy */ (4<<1)|1,   
#endif
   
   /* uint32_t twi_baudrate */ 100000,   
   /* uint8_t twi_slaveaddr */ 0x41
};
