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

#ifndef _pato_config_h_
#define _pato_config_h_

#ifdef __AVR_ATmega2560__
#define F_CPU 16000000
#endif
#ifdef __AVR_ATtiny2313__
#define F_CPU 8000000
#endif
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)
#define F_CPU 16000000
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
