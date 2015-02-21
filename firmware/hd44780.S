/** -*- asm -*-
 * \file
 *
 * \brief Low level interface for communication with HD44780
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

#define __ASSEMBLY__ 1

#include <avr/io.h>

#include "hd44780.h"

	.section .text

	.macro delay delta
	ldi	r18, \delta-1
50:	dec	r18
	brpl	50b
	.endm
	
	.macro bit_out vreg, bitnum, outnum
	sbi	HD44780_DATA_DDR, \outnum
	sbrs	\vreg, \bitnum
	cbi	HD44780_DATA_OUT, \outnum
	sbrc	\vreg, \bitnum
	sbi	HD44780_DATA_OUT, \outnum
	.endm
	
	
	.macro bit_in vreg, innum
	lsl	\vreg
	sbic HD44780_DATA_IN, \innum
	inc	\vreg
	.endm

	.func nibble_out
nibble_out:	
	sbi HD44780_CTRL_OUT, HD44780_EN
	delay 6
	bit_out r24, 0, HD44780_DB4
	bit_out r24, 1, HD44780_DB5
	bit_out r24, 2, HD44780_DB6
	bit_out r24, 3, HD44780_DB7
	cbi HD44780_CTRL_OUT, HD44780_EN
	delay 8
	ret
	.endfunc

	.func nibble_in
nibble_in:	
	sbi HD44780_CTRL_OUT, HD44780_EN
	delay 6
	bit_in r24, HD44780_DB7
	bit_in r24, HD44780_DB6
	bit_in r24, HD44780_DB5
	bit_in r24, HD44780_DB4
	cbi HD44780_CTRL_OUT, HD44780_EN
	delay 8
	ret
	.endfunc
	
	.func data_ddr_in
data_ddr_in:	
	cbi HD44780_DATA_DDR, HD44780_DB4
	cbi HD44780_DATA_DDR, HD44780_DB5
	cbi HD44780_DATA_DDR, HD44780_DB6
	cbi HD44780_DATA_DDR, HD44780_DB7
	ret
	.endfunc

	.func data_ddr_out
data_ddr_out:	
	sbi HD44780_DATA_DDR, HD44780_DB4
	sbi HD44780_DATA_DDR, HD44780_DB5
	sbi HD44780_DATA_DDR, HD44780_DB6
	sbi HD44780_DATA_DDR, HD44780_DB7
	ret
	.endfunc
	
	.func byte_out
byte_out:
	swap	r24
	call 	nibble_out
	swap	r24
	call 	nibble_out 
	ret
	.endfunc

	.func byte_in
byte_in:	
	clr r24
	call nibble_in
	call nibble_in
	ret
	.endfunc
	
	.global hd44780_reset
	.func hd44780_reset
hd44780_reset:
	sbi HD44780_CTRL_DDR, HD44780_RS
	sbi HD44780_CTRL_DDR, HD44780_RW
	sbi HD44780_CTRL_DDR, HD44780_EN
	sbi HD44780_CTRL_DDR, HD44780_RST
	call data_ddr_out
	
	cbi HD44780_DATA_OUT, HD44780_DB4
	cbi HD44780_DATA_OUT, HD44780_DB5
	cbi HD44780_DATA_OUT, HD44780_DB6
	cbi HD44780_DATA_OUT, HD44780_DB7

	cbi HD44780_CTRL_OUT, HD44780_RS
	cbi HD44780_CTRL_OUT, HD44780_RW
	cbi HD44780_CTRL_OUT, HD44780_EN	
	cbi HD44780_CTRL_OUT, HD44780_RST

	;; if only shutdown is required - exit
	cpi r24, 0xff
	brne 10f
	ret

10:
        ;; set reset pin up, wait till HD44780 completes boot
	delay 0xff
	sbi HD44780_CTRL_OUT, HD44780_RST
	mov r25, r24
	call hd44780_wait_busy

        ;; Power-on initialization, switching from 8-bit to 4-bit mode.
        ;; See [HD44780 Data-sheet](http://tinyurl.com/qxfogu9) see
        ;; "Initializing by Instruction" section Table 12
        
        ;; r24 (input param) is supposed to be loaded with FUNC_SET command
        ;; with 4-bit mode enabled. 
	mov r24, r25	
	swap r24
	call nibble_out
	delay 0x8

        ;; recover r24 again and send full command
	mov r24, r25
	call hd44780_ir_write
	call hd44780_wait_busy

        ;; recover r24 again and send full command again
        ;; @bug I am not sure why this is needed, but somethimes display
        ;; wouldn't enter 2-line mode without the second invocation.
	mov r24, r25
	call hd44780_ir_write
	call hd44780_wait_busy

	ret
	.endfunc
	
	.global hd44780_ir_write
	.func hd44780_ir_write
hd44780_ir_write:
	cbi HD44780_CTRL_OUT, HD44780_RS
	cbi HD44780_CTRL_OUT, HD44780_RW
	call byte_out
	ret
	.endfunc

	.global hd44780_wait_busy
	.func hd44780_wait_busy
hd44780_wait_busy:
	call data_ddr_in
	cbi HD44780_CTRL_OUT, HD44780_RS
	sbi HD44780_CTRL_OUT, HD44780_RW
10:
	call byte_in
	sbrc r24, 7
	rjmp 10b
	
	ret
	.endfunc
	
	.global hd44780_dr_write
	.func hd44780_dr_write
hd44780_dr_write:
	sbi HD44780_CTRL_OUT, HD44780_RS
	cbi HD44780_CTRL_OUT, HD44780_RW
	call byte_out
	ret
	.endfunc

	.global hd44780_dr_read
	.func hd44780_dr_read
hd44780_dr_read:
	sbi HD44780_CTRL_OUT, HD44780_RS
	sbi HD44780_CTRL_OUT, HD44780_RW
	call byte_in
	ret
	.endfunc
