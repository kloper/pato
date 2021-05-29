/** -*- C -*-
 * \file
 *
 * \brief Communication protocol for Pato
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

#ifndef _pato_protocol_h_
#define _pato_protocol_h_

/**
 * @defgroup pato_protocol Pato Communication Protocol
 * @brief How to communicate with Pato
 *
 * Pato defines very simple communication protocol that uses either UART
 * or I2C as a transport. The protocol is designed to minimize RAM usage.
 * It requires only 10 bytes of RAM for input and output buffers. 
 * 
 * Pato uses fixed size packets for receiving and transmitting. Each packet
 * consists of 5 bytes (see packet_t):
 *   1. One byte for command id.
 *   2. Two bytes for payload
 *   3. One bye for CRC8 calculated on three previous bytes.
 *   4. Zero byte.
 * 
 * Note that relatively large overhead for CRC and Zero byte is required since
 * I am planning to use displays connected over relatively long wires in noisy 
 * environment, this requires extra protocol reliability. Zero byte is reserved 
 * for future, to make possible recovery for partially received or corrupted 
 * packets.
 *
 * CRC8 algorithm is taken from Atmel util/crc16.h header file. Here is copy
 * of the C source code from the file:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~
 * uint8_t
 * _crc_ibutton_update(uint8_t crc, uint8_t data)
 *  {
 *	uint8_t i;
 *
 *	crc = crc ^ data;
 *	for (i = 0; i < 8; i++)
 *	{
 *	    if (crc & 0x01)
 *	        crc = (crc >> 1) ^ 0x8C;
 *	    else
 *	        crc >>= 1;
 *	}
 *
 *	return crc;
 *  }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * For input packets (sent from master to Pato) the packet_t::cmd must contain
 * a valid command id, While packet_t::arg0 and packet_t::arg1 are command 
 * arguments.
 * 
 * Output packets normally contain command replies. For command reply 
 * packet_t::cmd will contain success or failure flag and argument bytes will
 * contain extra information from command.
 * @{
 */

/**
 * @brief PING Command
 *
 * Implements aliveness check. Master sends a value that is incremented and 
 * sent in reply to this command.
 * 
 * Field | Request       | Reply
 * ------|---------------|---------------
 * cmd   | PATO_CMD_PING | PATO_REPLY_OK
 * arg0  | not used      | PATO_CMD_PING
 * arg1  | value         | value+1
 */
#define PATO_CMD_PING          0x00

/**
 * @brief DIRECT Command
 *
 * Direct access to low-level HD44780 API. In order to minimize parsing code
 * size, all direct access commands are implemented as sub-commands of a single
 * command.
 *
 * See PATO_DIRECT_CLR, PATO_DIRECT_HOME, PATO_DIRECT_EMS, PATO_DIRECT_DCTRL
 * PATO_DIRECT_SHIFT, PATO_DIRECT_FUNC, PATO_DIRECT_CGADDR, PATO_DIRECT_DDADDR,
 * PATO_DIRECT_BUSY_WAIT, PATO_DIRECT_WRITE, PATO_DIRECT_READ for specifics.
 */
#define PATO_CMD_DIRECT        0x01

/**
 * @brief RESET Command
 *
 * Reset or shutdown HD44780 display. Provides direct access to hd44780_reset()
 * function. 
 * 
 * Field | Request        | Reply
 * ------|----------------|---------------
 * cmd   | PATO_CMD_RESET | PATO_REPLY_OK
 * arg0  | cmd_source     | actual_cmd
 * arg1  | cmd            | PATO_CMD_RESET
 *
 * @param cmd_source allows to choose where to get parameter for hd44780_reset()
 *                   If 1 arg1 value will be used for cmd.
 *                   If 2 cmd will get value of 0xff meaning shutdown 
 *                   of the display.
 *                   Any other value will set cmd to its default value.
 * @param cmd custom cmd value when cmd_source equals 1. Ignored otherwise.
 * @param actual_cmd the command will return actual cmd value that was used.
 */
#define PATO_CMD_RESET         0x02

/**
 * @brief CONTRAST Command
 *
 * Change HD44780 display text contrast.
 *
 * @todo Not Implemented Yet
 */
#define PATO_CMD_CONTRAST      0x03

/**
 * @brief BACKLIGHT Command
 *
 * Change HD44780 display back-light strength.
 *
 * @todo Not Implemented Yet
 */
#define PATO_CMD_BACKLIGHT     0x04

/**
 * @brief PRINT SETADDR Command
 *
 * Set current address inside internal print buffer to a specific position.
 * 
 * Field | Request                | Reply
 * ------|------------------------|-----------------------
 * cmd   | PATO_CMD_PRINT_SETADDR | PATO_REPLY_OK
 * arg0  | addr_low               | status
 * arg1  | addr_high              | PATO_CMD_PRINT_SETADDR
 *
 * @invariant
 * `(addr_high << 8) | addr_low` must be in range from 0 to the print buffer
 * size minus 1.
 *
 * @param addr_low low byte of the print buffer address
 * @param addr_high high byte of the print buffer address
 * @param status true if the operation succeeded (address is a valid index)
 *               false otherwise.
 */
#define PATO_CMD_PRINT_SETADDR 0x05

/**
 * @brief PRINT GETADDR Command
 *
 * Retrieve current address inside internal print buffer.
 * 
 * Field | Request                | Reply
 * ------|------------------------|-----------------------
 * cmd   | PATO_CMD_PRINT_GETADDR | PATO_REPLY_OK
 * arg0  | not used               | addr_low
 * arg1  | not used               | addr_high
 *
 * @param addr_low low byte of the print buffer address
 * @param addr_high high byte of the print buffer address
 */
#define PATO_CMD_PRINT_GETADDR 0x06

/**
 * @brief PRINT PUT Command
 *
 * Put a pair of bytes in print buffer at current address position,
 * autoincrementing the current address after each byte. If either byte is
 * supposed to fall out of the buffer the entire operation fails and none
 * of them is written.
 * 
 * Field | Request            | Reply
 * ------|--------------------|-----------------------
 * cmd   | PATO_CMD_PRINT_PUT | PATO_REPLY_OK
 * arg0  | byte0              | status
 * arg1  | byte1              | PATO_CMD_PRINT_PUT
 *
 * @param byte0 first byte to be written, the current address is incremented
 * @param byte1 second byte to be written, the current address is incremented
 * @param status true if the operation succeeded (both bytes are written)
 *               false otherwise.
 */
#define PATO_CMD_PRINT_PUT     0x07

/**
 * @brief PRINT PUT PTR Command
 *
 * This is a special version of \ref PATO_CMD_PRINT_PUT. It will treat incoming
 * arg0 and arg1 as a single uint16_t offset from beginning of the print buffer.
 * It will add the offset to the pointer to the beginning of the print buffer
 * and save the resulting pointer at the current address. 
 *
 * This is required for treating "%s" formats that require both pointer and
 * actual string to be delvered from master.
 * 
 * Field | Request                | Reply
 * ------|------------------------|-----------------------
 * cmd   | PATO_CMD_PRINT_PUT_PTR | PATO_REPLY_OK
 * arg0  | offset low byte        | status
 * arg1  | offset high byte       | PATO_CMD_PRINT_PUT_PTR
 *
 * @param byte0 low byte of the offset
 * @param byte1 high byte of the offset
 * @param status true if the operation succeeded (enough space in the buffer 
 *        and the input offset is valid) false otherwise.
 */
#define PATO_CMD_PRINT_PUT_PTR 0x09

/**
 * @brief PRINT COMMIT Command
 *
 * Send content of the print buffer to HD44780 display.
 * For multiline displays this will automatically arrange content of the
 * buffer according with the line interleaving.
 *
 * Depending of Pato's features compiled in, this command will also process 
 * terminal control characters.
 *
 * If compiled with printf support, this command will also interpret the 
 * buffer as a printf format string separated by '\0' character with its 
 * parameters.
 *
 * Arguments are treated as offset in the print buffer, see 
 * hd44780_print_commit() and @ref print_buffer_t for more information.
 *
 * Field | Request               | Reply
 * ------|-----------------------|-----------------------
 * cmd   | PATO_CMD_PRINT_COMMIT | PATO_REPLY_OK
 * arg0  | offset low byte       | status
 * arg1  | offset high byte      | PATO_CMD_PRINT_COMMIT
 *
 * @param arg0 low byte of the offset
 * @param arg1 high byte of the offset
 * @param status true if the operation succeeded false otherwise. The operation
 *               may fail only if there is no '\0' character terminating the
 *               buffer, or separating format from parameters.
 */
#define PATO_CMD_PRINT_COMMIT  0x08

/**
 * @brief DIRECT CLR Command
 *
 * HD44780 direct low level access to Clear Screen command.
 * 
 * Field | Request         | Reply
 * ------|-----------------|---------------
 * cmd   | PATO_CMD_DIRECT | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_CLR | addr
 * arg1  | not used        | not used
 *
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 * @see hd44780_ir_write()
 * @see HD44780_CMD_CLEAR_SCREEN
 */
#define PATO_DIRECT_CLR       0x01

/**
 * @brief DIRECT HOME Command
 *
 * HD44780 direct low level access to Return Home command.
 * 
 * Field | Request          | Reply
 * ------|------------------|---------------
 * cmd   | PATO_CMD_DIRECT  | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_HOME | addr
 * arg1  | not used         | not used
 *
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 * @see hd44780_ir_write()
 * @see HD44780_CMD_RETURN_HOME
 */
#define PATO_DIRECT_HOME      0x02

/**
 * @brief DIRECT EMS Command
 *
 * HD44780 direct low level access to Entry Mode Set command.
 * 
 * Field | Request         | Reply
 * ------|-----------------|---------------
 * cmd   | PATO_CMD_DIRECT | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_EMS | addr
 * arg1  | args            | not used
 *
 * @param args bitmask that defines EMS arguments.
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 * @see hd44780_ir_write()
 * @see HD44780_CMD_EMS
 * @see HD44780_CMD_EMS_INCR
 * @see HD44780_CMD_EMS_SHIFT
 */
#define PATO_DIRECT_EMS       0x03

/**
 * @brief DIRECT DCTRL Command
 *
 * HD44780 direct low level access to Display Control command.
 * 
 * Field | Request           | Reply
 * ------|-------------------|---------------
 * cmd   | PATO_CMD_DIRECT   | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_DCTRL | addr
 * arg1  | args              | not used
 *
 * @param args bitmask that defines DISPLAY arguments.
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 * @see hd44780_ir_write()
 * @see HD44780_CMD_DISPLAY
 * @see HD44780_CMD_DISPLAY_ON
 * @see HD44780_CMD_DISPLAY_CURS_ON
 * @see HD44780_CMD_DISPLAY_CURS_BLINK
 */
#define PATO_DIRECT_DCTRL     0x04

/**
 * @brief DIRECT SHIFT Command
 *
 * HD44780 direct low level access to Shift Control command.
 * 
 * Field | Request           | Reply
 * ------|-------------------|---------------
 * cmd   | PATO_CMD_DIRECT   | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_SHIFT | addr
 * arg1  | args              | not used
 *
 * @param args bitmask that defines SHIFT arguments.
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 * @see hd44780_ir_write()
 * @see HD44780_CMD_SHIFT
 * @see HD44780_CMD_SHIFT_DISPLAY
 * @see HD44780_CMD_SHIFT_RIGHT
 */
#define PATO_DIRECT_SHIFT     0x05

/**
 * @brief DIRECT FUNC Command
 *
 * HD44780 direct low level access to Function Set command.
 * 
 * Field | Request          | Reply
 * ------|------------------|---------------
 * cmd   | PATO_CMD_DIRECT  | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_FUNC | addr
 * arg1  | args             | not used
 *
 * @param args bitmask that defines FUNC arguments.
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 * @see hd44780_ir_write()
 * @see HD44780_CMD_FUNC_SET
 * @see HD44780_CMD_FUNC_8BIT
 * @see HD44780_CMD_FUNC_2LINES
 * @see HD44780_CMD_FUNC_5x10
 */
#define PATO_DIRECT_FUNC      0x06

/**
 * @brief DIRECT CGADDR Command
 *
 * HD44780 direct low level access to Set CGRAM Address command.
 * 
 * Field | Request            | Reply
 * ------|--------------------|---------------
 * cmd   | PATO_CMD_DIRECT    | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_CGADDR | addr
 * arg1  | cg_addr            | not used
 *
 * @param cg_addr CG RAM address to be set
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 * @see hd44780_ir_write()
 * @see HD44780_CMD_SET_CGRAM_ADDR
 */
#define PATO_DIRECT_CGADDR    0x07

/**
 * @brief DIRECT DDADDR Command
 *
 * HD44780 direct low level access to Set DDRAM Address command.
 * 
 * Field | Request            | Reply
 * ------|--------------------|---------------
 * cmd   | PATO_CMD_DIRECT    | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_DDADDR | addr
 * arg1  | dd_addr            | not used
 *
 * @param dd_addr DD RAM address to be set
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 * @see hd44780_ir_write()
 * @see HD44780_CMD_SET_DDRAM_ADDR
 */
#define PATO_DIRECT_DDADDR    0x08

/**
 * @brief DIRECT BUSY WAIT Command
 *
 * HD44780 direct low level access to Busy Wait command.
 * 
 * Field | Request               | Reply
 * ------|-----------------------|---------------
 * cmd   | PATO_CMD_DIRECT       | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_BUSY_WAIT | addr
 * arg1  | not used              | not used
 *
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 */
#define PATO_DIRECT_BUSY_WAIT 0x09

/**
 * @brief DIRECT WRITE Command
 *
 * HD44780 direct low level access to Data Write command.
 * 
 * Field | Request           | Reply
 * ------|-------------------|---------------
 * cmd   | PATO_CMD_DIRECT   | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_WRITE | addr
 * arg1  | data              | not used
 *
 * @param data single byte to be written to HD44780 Data Register
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 */
#define PATO_DIRECT_WRITE     0x0A

/**
 * @brief DIRECT READ Command
 *
 * HD44780 direct low level access to Data Read command.
 * 
 * Field | Request          | Reply
 * ------|------------------|---------------
 * cmd   | PATO_CMD_DIRECT  | PATO_REPLY_OK
 * arg0  | PATO_DIRECT_READ | addr
 * arg1  | none             | data
 *
 * @param data single byte received from HD44780 Data Register
 * @param addr current HD44780 address as returned by hd44780_wait_busy()
 */
#define PATO_DIRECT_READ      0x0B

/**
 * @brief Reply Success Indicator
 *
 * Value to be put in packet_t::cmd field in reply packets to indicate 
 * command success.
 */
#define PATO_REPLY_OK    0xfe

/**
 * @brief Reply Error Indicator
 *
 * Value to be put in packet_t::cmd field in reply packets to indicate 
 * command failure.
 */
#define PATO_REPLY_ERROR 0xff


/**
 * @brief CRC Error
 *
 * Value to be put in packet_t::arg0 field in reply packets to indicate 
 * request packet CRC error.
 */
#define PATO_ERROR_CRC    0x01

/**
 * @brief Bad Command Error
 *
 * Value to be put in packet_t::arg0 field in reply packets to indicate 
 * unknown command request.
 */
#define PATO_ERROR_BADCMD 0x02

/**
 * @brief Bad Argument Error
 *
 * Value to be put in packet_t::arg0 field in reply packets to indicate 
 * invalid command argument.
 */
#define PATO_ERROR_BADARG 0x03

/**
 * @brief Bad Packet Separator Error
 *
 * Value to be put in packet_t::arg0 field in reply packets to indicate 
 * that last byte in request packet is not zero. 
 */
#define PATO_ERROR_SEP    0x04

/**
 * @brief Input/Output Packet Structure
 */
typedef struct _packet {
   uint8_t cmd; /**< For input packets must contain a known command id 
                   (see PATO_CMD_DIRECT, PATO_CMD_PING, PATO_CMD_RESET, etc...)
                   For output packets must be either PATO_REPLY_OK or
                   PATO_REPLY_ERROR */
   uint8_t arg0; /**< Command-specific payload byte #0 */
   uint8_t arg1; /**< Command-specific payload byte #1 */
   uint8_t crc; /**< CRC8 as defined by _crc_ibutton_update() from Atmel's
                   header util/crc16.h */
   uint8_t zero; /**< Zero byte. Must be always zero */
} packet_t;

/** @} */

#endif /* _pato_protocol_h_ */
