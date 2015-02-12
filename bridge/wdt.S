/** -*- asm -*-
 * \file
 *
 * \brief Control WatchDog Timer for timeout purposes
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

	.global WDT_vect
	.func WDT_vect
WDT_vect:
	reti
	.endfunc
	
	.global wdt_enable
	.func wdt_enable
	;; Input:
	;; r24 - prescale
wdt_enable:
	cli

	andi	r24, 0x7
	ori	r24, (1<<WDIE)
	
	wdr
	in 	r25, _SFR_IO_ADDR(MCUSR)
	andi 	r25, ~(1<<WDRF)
	out 	_SFR_IO_ADDR(MCUSR), r25
	
	lds 	r25, WDTCSR
	ori 	r25, (1<<WDCE) | (1<<WDE) 
	sts 	WDTCSR, r25
	sts 	WDTCSR, r24

	reti
	.endfunc

	.global wdt_disable
	.func wdt_disable
wdt_disable:
	cli
	lds 	r25, WDTCSR
	ori 	r25, (1<<WDCE) | (1<<WDE)
	sts 	WDTCSR, r25
	sts 	WDTCSR, r1
	reti
	.endfunc

	.global wdt_reset
	.func wdt_reset
wdt_reset:
	wdr
	ret
	.endfunc


	

