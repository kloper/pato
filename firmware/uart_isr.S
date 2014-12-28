/* -*- asm -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * uart_isr.S -- UART ISR routine 
 *
 */

#define __ASSEMBLY__ 1

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"
	
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

	
