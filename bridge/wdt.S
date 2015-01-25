/* -*- asm -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * wdt.S - control WatchDog Timer for timeout purposes
 *
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


	

