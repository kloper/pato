/** -*- C -*-
 * \file
 *
 * \brief Interface to HD44780 controller
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

#ifndef _pato_hd44780_h_
#define _pato_hd44780_h_

#define HD44780_CTRL_PORT PORTA
#define HD44780_CTRL_DDR (_SFR_IO_ADDR(HD44780_CTRL_PORT) - 1)
#define HD44780_CTRL_OUT (_SFR_IO_ADDR(HD44780_CTRL_PORT))
#define HD44780_CTRL_IN  (_SFR_IO_ADDR(HD44780_CTRL_PORT) - 2)

#define HD44780_RS PA0
#define HD44780_RW PA1
#define HD44780_EN PA2
#define HD44780_RST PA3

#define HD44780_DATA_PORT PORTC
#define HD44780_DATA_DDR (_SFR_IO_ADDR(HD44780_DATA_PORT) - 1)
#define HD44780_DATA_OUT (_SFR_IO_ADDR(HD44780_DATA_PORT))
#define HD44780_DATA_IN  (_SFR_IO_ADDR(HD44780_DATA_PORT) - 2)

#define HD44780_DB4 PC4
#define HD44780_DB5 PC5
#define HD44780_DB6 PC6
#define HD44780_DB7 PC7
#define HD44780_BUSY PC7

#define HD44780_CMD_CLEAR_SCREEN       0x01
#define HD44780_CMD_RETURN_HOME        0x02
#define HD44780_CMD_EMS                0x04
#define HD44780_CMD_EMS_INCR           0x02
#define HD44780_CMD_EMS_SHIFT          0x01
#define HD44780_CMD_DISPLAY            0x08
#define HD44780_CMD_DISPLAY_ON         0x04
#define HD44780_CMD_DISPLAY_CURS_ON    0x02
#define HD44780_CMD_DISPLAY_CURS_BLINK 0x01
#define HD44780_CMD_SHIFT              0x10
#define HD44780_CMD_SHIFT_DISPLAY      0x08
#define HD44780_CMD_SHIFT_RIGHT        0x04
#define HD44780_CMD_FUNC_SET           0x20
#define HD44780_CMD_FUNC_8BIT          0x10
#define HD44780_CMD_FUNC_2LINES        0x08
#define HD44780_CMD_FUNC_5x10          0x04
#define HD44780_CMD_SET_CGRAM_ADDR     0x40
#define HD44780_CMD_SET_DDRAM_ADDR     0x80

#ifndef __ASSEMBLY__
extern void hd44780_reset(uint8_t cmd);
extern void hd44780_ir_write(uint8_t cmd);
extern uint8_t hd44780_wait_busy(void);
extern void hd44780_dr_write(uint8_t value);
extern uint8_t hd44780_dr_read(void);
#endif /* __ASM__ */

#endif /* _pato_hd44780_h_ */
