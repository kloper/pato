/** -*- asm -*-
 * \file
 *
 * \brief UART ISR routine
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
#include <avr/interrupt.h>

#include "config.h"
#include "uart.h"

#if defined(HAVE_UART)
	
	.text	

	;; The only advantage of this ISR over the one implemented in
	;; uart.c is low stack profile. It requires only 4 bytes to be
	;; pushed on stack before working.
	.global USART0_RX_vect
	.func USART0_RX_vect
USART0_RX_vect:
	push 	r30
	push	r31
	push	r24
	push	r25

	;; g_uart_inbuf -> Z
	;; g_uart_inbuf_head -> r25
	ldi	r30, lo8(g_uart_inbuf)
	ldi	r31, hi8(g_uart_inbuf)
	lds	r25, g_uart_inbuf_head

	;; g_uart_inbuf[UART_INBUF_SIZE-1] = 0xff
	ldi	r24, 0xff
	std	Z+(UART_INBUF_SIZE-1), r24

uart_isr_loop:
	;; UCSR0A -> r24
	lds	r24, UCSR0A
	;;  if RXC0 is not set - exit
	sbrs	r24, RXC0
	rjmp	uart_isr_exit

	;; if there is a problem with received byte - skip it
	andi	r24, ((1<<FE0)|(1<<DOR0)|(1<<UPE0)) 
	breq	10f
	;; skip actiually requires UDR0 read
	lds	r24, UDR0
	rjmp	uart_isr_loop

10:
	;; Z + g_uart_inbuf_head -> Z
	clr	r24
	add	r30, r25
	adc	r31, r24

	;; UDR0 -> g_uart_inbuf[g_uart_inbuf_head]
	lds	r24, UDR0
	st	Z, r24

	;; g_uart_inbuf_head = (g_uart_inbuf_head+1) % UART_INBUF_SIZE
	inc	r25
	cpi	r25, UART_INBUF_SIZE
	brlt	uart_isr_loop

	clr	r25
	rjmp	uart_isr_loop
	
uart_isr_exit:
	;; store g_uart_inbuf_head 
	sts	g_uart_inbuf_head, r25
	
	pop	r25
	pop	r24
	pop	r31
	pop	r30
	reti
	.endfunc

#endif /* HAVE_UART */
