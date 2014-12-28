/* -*- asm -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * timeout.S -- use 8-bit counter #0 to track time
 *
 */

#define __ASSEMBLY__ 1

#include <avr/io.h>

	.global to_start
	.func to_start
	;; Input:
	;; r24 - prescale
to_start:
	andi	r24, 0x7
	sts	TCCR0B, r1	
	sts	TCNT0, r1	
	sbi	_SFR_IO_ADDR(TIFR0), TOV0
	sts	TCCR0A, r1
	sts	TCCR0B, r24	
	ret
	.endfunc

	.global to_check
	.func to_check
to_check:
	clr 	r24
	sbic	_SFR_IO_ADDR(TIFR0), TOV0 
	inc	r24
	ret
	.endfunc

	
