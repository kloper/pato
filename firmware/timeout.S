/** -*- asm -*-
 * \file
 *
 * \brief 8-bit counter #0 to track time
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

#ifndef TIFR0        
#define TIFR0 TIFR
#endif
        
	.global to_start
	.func to_start
	;; Input:
	;; r24 - prescale
to_start:
        cli
	andi	r24, 0x7
	sts	TCCR0B, r1	
	sts	TCNT0, r1	
	;; sbi	_SFR_IO_ADDR(TIFR0), TOV0
        lds     r25, TIFR0
        ori     r25, (1<<TOV0)
        sts     TIFR0, r25
	sts	TCCR0A, r1
	sts	TCCR0B, r24	
	reti
	.endfunc

	.global to_check
	.func to_check
to_check:
        cli
	clr 	r24
	;; sbic	_SFR_IO_ADDR(TIFR0), TOV0 
        lds     r25, TIFR0
        sbrc    r25, TOV0
	inc	r24
	reti
	.endfunc

	
