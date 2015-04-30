/** -*- C -*-
 * \file
 *
 * \brief printf functionality for Pato
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

#ifndef _pato_print_h_
#define _pato_print_h_

#if defined(HAVE_PRINT)

#define PATO_PRINT_BUFFER_SIZE 80

/**
 * @brief Print buffer
 *
 * A buffer that holds all the information required for formatting text
 * that is displayed by the controlled HD44780. 
 * 
 * Without printf support, this buffer shall hold just a null terminated 
 * string to be copied to HD44780 memory.
 *
 * With printf support the buffer shall hold both pintf's format string and
 * its parameters in binary format ready to be consumed by the printf() 
 * function. 
 *
 * It is responsibility of Pato's user to construct a valid content in the 
 * buffer either using hd44780_print_put(), hd44780_print_set_addr(), 
 * hd44780_print_commit() functions or by using @ref PATO_CMD_PRINT_PUT,
 * @ref PATO_CMD_PRINT_SETADDR, @ref PATO_CMD_PRINT_COMMIT and other print 
 * APIs. It possible for the user to change only parts of buffer by changing
 * the current address.
 *
 * The current print address is an offset in the buffer that defines the next
 * byte to be filled by hd44780_print_put(). Normally the current address is
 * autoincremented after each byte written.
 * 
 */
typedef struct _print_buffer {
   uint16_t addr; /**< Current print address - offset in the buffer 
                     that determines position of next filled byte */
   uint8_t  buffer[PATO_PRINT_BUFFER_SIZE]; /**< Print buffer */
} print_buffer_t;

/**
 * @brief Print buffer singletone 
 */
extern print_buffer_t g_print_buffer;

extern uint8_t hd44780_print_commit(uint16_t offset);
extern uint8_t hd44780_print_set_addr(uint16_t addr);
extern uint16_t hd44780_print_get_addr();
extern uint8_t hd44780_print_put(uint8_t arg0, uint8_t arg1);
extern uint8_t hd44780_print_put_ptr(uint16_t offset);
extern void hd44780_print_reset();

#endif /* HAVE_PRINT */

#endif /* _pato_print_h_ */
