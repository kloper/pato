/* -*- asm -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * hd44780.S -- low level interface for communication with HD44780
 *
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

	.func data_ddr_in
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
	delay 0xff
	sbi HD44780_CTRL_OUT, HD44780_RST
	mov r25, r24
	call hd44780_wait_busy

	mov r24, r25	
	swap r24
	call nibble_out
	delay 0x8

	mov r24, r25
	call hd44780_ir_write
	call hd44780_wait_busy

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
