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

/**
 * @defgroup hd44780_pins Atmega to HD44780 physical wiring
 * @brief How to connect Atmega loaded with Pato to HD44780
 *
 * In order to minimize number of physical connections with the display
 * Pato uses 4-bit data interface. For exact specification of the interface
 * please refer to [HD44780 Data-sheet](http://tinyurl.com/qxfogu9).
 * 
 * In order to minimize amount of code required to communicate with display 
 * controller, pins are grouped in two groups: control pins and data pins.
 *
 * Each group must belong to same GPIO port. Control pins set consists of:
 *   * RS - Register Select
 *   * RW - Read/Write 
 *   * EN - Enable (aka clock)
 *   * RST - reset. This pin is connected to Vdd pin of the display and is used
 *           for supplying power to display controller and cold reset.
 *
 * Data pins are DB4 to DB7, when the latter one is used also as BUSY bit of 
 * the display. The BUSY bit is set to HIGH when display controller is busy
 * while performing a command.
 *
 * @{ 
 * @def HD44780_CTRL_PORT
 * Must be set to GPIO port name (e.g. PORTA). Specifies Control Port - GPIO
 * port containing all control pins. 
 *
 * @def HD44780_CTRL_DDR
 * Specifies Data Direction Register (DDR) for the Control Port. It is 
 * suitable for assembly 'in' and 'out' commands.
 * Derived from HD44780_CTRL_PORT. 
 *
 * @def HD44780_CTRL_OUT
 * Specifies Control Port Register number suitable for assembly
 * 'in' and 'out' commands.
 * Derived from HD44780_CTRL_PORT. 
 *
 * @def HD44780_CTRL_IN
 * Specifies Input Register number for Control Port. It is suitable for 
 * assembly 'in' and 'out' commands.
 * Derived from HD44780_CTRL_PORT. 
 *
 * @def HD44780_DATA_PORT
 * Must be set to GPIO port name (e.g. PORTC). Specifies Data Port - GPIO port
 * containing all data pins.
 *
 * @def HD44780_DATA_DDR
 * Specifies Data Direction Register (DDR) for the Data Port. It is 
 * suitable for assembly 'in' and 'out' commands.
 * Derived from HD44780_DATA_PORT. 
 *
 * @def HD44780_DATA_OUT
 * Specifies Data Port Register number suitable for assembly
 * 'in' and 'out' commands.
 * Derived from HD44780_DATA_PORT. 
 *
 * @def HD44780_DATA_IN
 * Specifies Input Register number for Data Port. It is suitable for 
 * assembly 'in' and 'out' commands.
 * Derived from HD44780_DATA_PORT. 
 *
 * @def HD44780_RS
 * Specifies pin in HD44780_CTRL_PORT that is connected to RS pin of HD44780.
 *
 * @def HD44780_RW
 * Specifies pin in HD44780_CTRL_PORT that is connected to RW pin of HD44780.
 *
 * @def HD44780_EN
 * Specifies pin in HD44780_CTRL_PORT that is connected to EN pin of HD44780.
 *
 * @def HD44780_RST
 * Specifies pin in HD44780_CTRL_PORT that is connected to Vdd pin of HD44780.
 * This pin is actually used to supply power to of HD44780's logic and for
 * cold resetting.
 *
 * @def HD44780_DB4
 * Specifies pin in HD44780_DATA_PORT that is connected to DB4 pin of HD44780.
 *
 * @def HD44780_DB5
 * Specifies pin in HD44780_DATA_PORT that is connected to DB5 pin of HD44780.
 *
 * @def HD44780_DB6
 * Specifies pin in HD44780_DATA_PORT that is connected to DB6 pin of HD44780.
 *
 * @def HD44780_DB7
 * Specifies pin in HD44780_DATA_PORT that is connected to DB7 pin of HD44780.
 *
 * @def HD44780_BUSY
 * Alias or identical to HD44780_DB7.
 *
 * @note for future
 * The V0 pin of HD44780 has limited usability range - its from 0 to 1V approx.
 *
 * @} 
 */

#ifdef __AVR_ATmega2560__
#define HD44780_CTRL_PORT PORTA

#define HD44780_RS PA0
#define HD44780_RW PA1
#define HD44780_EN PA2
#define HD44780_RST PA3

#define HD44780_DATA_PORT PORTC

#define HD44780_DB4 PC4
#define HD44780_DB5 PC5
#define HD44780_DB6 PC6
#define HD44780_DB7 PC7
#endif
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)
#define HD44780_CTRL_PORT PORTD

#define HD44780_RS PD5
#define HD44780_RW PD6
#define HD44780_EN PD7
#define HD44780_RST PD4

#define HD44780_DATA_PORT PORTC

#define HD44780_DB4 PC0
#define HD44780_DB5 PC1
#define HD44780_DB6 PC2
#define HD44780_DB7 PC3
#endif
#if defined(__AVR_ATtiny2313__) 
#define HD44780_CTRL_PORT PORTB

#define HD44780_RS PB1
#define HD44780_RW PB2
#define HD44780_EN PB3
#define HD44780_RST PB4

#define HD44780_DATA_PORT PORTD

#define HD44780_DB4 PD2
#define HD44780_DB5 PD3
#define HD44780_DB6 PD4
#define HD44780_DB7 PD5
#endif

#define HD44780_BUSY HD44780_DB7

#define HD44780_CTRL_DDR (_SFR_IO_ADDR(HD44780_CTRL_PORT) - 1)
#define HD44780_CTRL_OUT (_SFR_IO_ADDR(HD44780_CTRL_PORT))
#define HD44780_CTRL_IN  (_SFR_IO_ADDR(HD44780_CTRL_PORT) - 2)

#define HD44780_DATA_DDR (_SFR_IO_ADDR(HD44780_DATA_PORT) - 1)
#define HD44780_DATA_OUT (_SFR_IO_ADDR(HD44780_DATA_PORT))
#define HD44780_DATA_IN  (_SFR_IO_ADDR(HD44780_DATA_PORT) - 2)

/**
 * @defgroup hd44780_cmds Low level HD44780 interface 
 * @brief Internal low level HD44780 device driver
 *
 * Pato provides a small low-level library for controlling HD44780-based 
 * display. It can be used as a snad-alone module for other projects. 
 * It is imlemented in assembly with a goal to minimize flash memory usage 
 * and stack usage. 
 * 
 * The low-level library for HD44780 is very simple to use, but I recommend 
 * reading [HD44780 Data-sheet](http://tinyurl.com/qxfogu9) beforehand.
 * I will try to explain the basic library functinality by example below.
 * ~~~~~~~~~~~~~~~~~~~~~~
 * #include "hd44780.h"
 *
 * void main() 
 * {
 *    int i;
 * 
 *    hd44780_reset(HD44780_CMD_FUNC_SET  |
 *                  HD44780_CMD_FUNC_2LINES);
 *
 *    hd44780_ir_write(HD44780_CMD_DISPLAY         |
 *                     HD44780_CMD_DISPLAY_ON      |
 *                     HD44780_CMD_DISPLAY_CURS_ON |
 *                     HD44780_CMD_DISPLAY_CURS_BLINK );
 *    hd44780_wait_busy();
 * 
 *    hd44780_ir_write(HD44780_CMD_EMS |
 *                     HD44780_CMD_EMS_INCR);
 *    hd44780_wait_busy();
 * 
 *    for(i=0; i< 10; i++) {
 *        hd44780_dr_write('A'+i);
 *        hd44780_wait_busy();
 *    }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~
 * 
 * The first thing we do is reset the display. The hd44780_reset() function
 * is equvalent to a power-off power-on dequence followed by
 * hd44780_ir_write(HD44780_CMD_FUNC_SET...). This combination is required
 * for seamless switch to 4-bit mode from default 8-mode.
 *
 * Next, Display Control command is sent. It turns display and blinking cursor.
 * 
 * Next, Entry Mode Set command is sent. It turns cursor movement and defines
 * that cursor will move right automatically.
 *
 * Finally, we make a loop of 10 writes that will show 'ABCDEFGHIJ' on the
 * display.
 *
 * Note that every command except hd44780_reset() is followed by 
 * hd44780_wait_busy() call. This is because calling other HD44780 commands
 * while the controller is busy executing the current one will probably lead
 * to skipping of the commands. The reset does busy waiting internally so it 
 * does not require calling hd44780_wait_busy().
 * 
 * @{
 * @fn void hd44780_reset(uint8_t cmd)
 * @brief Cold reset display or shutdown
 *
 * This function will turn power off and then depending on its parameter
 * will trun power on or keep the display off. After power-on, it is supposed 
 * to make first communication with HD44780 and switch it to 4-bit mode. In 
 * order to accomplish this the cmd parameter must contain a valid Function
 * Control command. For example:
 * ~~~~~~~~~~~~~~~~~~~~
 * hd44780_reset(HD44780_CMD_FUNC_SET|HD44780_CMD_FUNC_2LINES);
 * ~~~~~~~~~~~~~~~~~~~~
 * Note that DL bit in the command is LOW, requiring 4-bit interface. The 
 * hd44780_reset() will essentially issue a call equivalent to 
 * `hd44780_ir_write(HD44780_CMD_FUNC_SET|HD44780_CMD_FUNC_2LINES)`
 * command, but the actual communication sequence and timing
 * is different from hd44780_ir_write() because 8-bit (default) 
 * to 4-bit data bus switch.
 * @param[in] cmd 
 * @parblock 
 * 0xff display will be shut-down. 
 *
 * Any value other than 0xff must be a valid Function Control command with DL
 * bit LOW (4-bit data bus).
 * @endparblock
 *
 * @fn void hd44780_ir_write(uint8_t cmd)
 * @brief Write to HD44780 Instruction Register
 *
 * Initiates command execution. Call to hd44780_wait_busy() is required to 
 * ensure that the command is completed.
 * @param[in] cmd 
 * @parblock 
 * Command byte as defined by HD44780 protocol. See "Instructions" section 
 * in the [HD44780 Data-sheet](http://tinyurl.com/qxfogu9).
 * @endparblock
 *
 * @fn uint8_t hd44780_wait_busy(void);
 * @brief Busy wait for PD7 to become LOW
 *
 * When HD44780's DB7 is HIGH, the display controller is busy. This function
 * polls state of the pin defined by #HD44780_BUSY macro waiting for LOW value.
 * @return Current Address Counter value
 *
 * @fn void hd44780_dr_write(uint8_t value)
 * @brief Write to HD44780 Data Register
 *
 * Writes a single byte value to Data Register. Call to hd44780_wait_busy() 
 * is required for to make ensure that the command is completed.
 * @param[in] value Single byte value to be written.
 *
 * @fn uint8_t hd44780_dr_read(void)
 * @brief Write to HD44780 Data Register
 *
 * Read a single byte value from Data register. Call to hd44780_wait_busy() 
 * is required for to make ensure that the command is completed.
 * @return Single byte value of Data Register.
 *
 */

/** Clear Screen command. 
    To be used as a parameter of hd44780_ir_write() */
#define HD44780_CMD_CLEAR_SCREEN       0x01
/** Return Home command. 
    To be used as a parameter of hd44780_ir_write() */
#define HD44780_CMD_RETURN_HOME        0x02
/** Entry Mode Set (EMS) command. 
    To be used as a parameter of hd44780_ir_write() */
#define HD44780_CMD_EMS                0x04
/** EMS Cursor/Display Direction bit mask. 
    To be used in conjunction with HD44780_CMD_EMS. */
#define HD44780_CMD_EMS_INCR           0x02 
/** EMS Display Shift bit mask. 
    To be used in conjunction with HD44780_CMD_EMS. */
#define HD44780_CMD_EMS_SHIFT          0x01
/** Display Control command. 
    To be used as a parameter of hd44780_ir_write() */
#define HD44780_CMD_DISPLAY            0x08
/** Display On/Off bit mask. 
    To be used in conjunction with HD44780_CMD_DISPLAY. */
#define HD44780_CMD_DISPLAY_ON         0x04
/** Cursor On/Off bit mask. 
    To be used in conjunction with HD44780_CMD_DISPLAY. */
#define HD44780_CMD_DISPLAY_CURS_ON    0x02
/** Cursor Blinking On/Off bit mask. 
    To be used in conjunction with HD44780_CMD_DISPLAY. */
#define HD44780_CMD_DISPLAY_CURS_BLINK 0x01
/** Shift Control command. 
    To be used as a parameter of hd44780_ir_write() */
#define HD44780_CMD_SHIFT              0x10
/** Cursor/Display Shift bit mask. 
    To be used in conjunction with HD44780_CMD_SHIFT. */
#define HD44780_CMD_SHIFT_DISPLAY      0x08
/** Left/Right Shift bit mask. 
    To be used in conjunction with HD44780_CMD_SHIFT. */
#define HD44780_CMD_SHIFT_RIGHT        0x04
/** Function Set command. 
    To be used as a parameter of hd44780_ir_write() */
#define HD44780_CMD_FUNC_SET           0x20
/** 4-bit/8-bit Data Bus Mode bit mask. 
    To be used in conjunction with HD44780_CMD_FUNC_SET. */
#define HD44780_CMD_FUNC_8BIT          0x10
/** 1-line/2-lines Display Mode bit mask. 
    To be used in conjunction with HD44780_CMD_FUNC_SET. */
#define HD44780_CMD_FUNC_2LINES        0x08
/** 5x10/5x8 Font Mode bit mask. 
    To be used in conjunction with HD44780_CMD_FUNC_SET. */
#define HD44780_CMD_FUNC_5x10          0x04
/** Set CG RAM Address command. 
    To be used as a parameter of hd44780_ir_write() */
#define HD44780_CMD_SET_CGRAM_ADDR     0x40
/** Set DD RAM Address command. 
    To be used as a parameter of hd44780_ir_write() */
#define HD44780_CMD_SET_DDRAM_ADDR     0x80

/** @} */

#ifndef __ASSEMBLY__
extern void hd44780_reset(uint8_t cmd);
extern void hd44780_ir_write(uint8_t cmd);
extern uint8_t hd44780_wait_busy(void);
extern void hd44780_dr_write(uint8_t value);
extern uint8_t hd44780_dr_read(void);
#endif /* __ASM__ */

#endif /* _pato_hd44780_h_ */
