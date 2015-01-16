/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * uart.c - UART side of the UART-I2C bridge
 *
 */

#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "config.h"
#include "uart.h"

static uint8_t g_uart_inbuf_head = 0;
static uint8_t g_uart_inbuf[UART_INBUF_SIZE];
static uint8_t g_uart_outbuf[UART_OUTBUF_SIZE];

#if defined(USART0_RX_vect)
#define USART_RX_vect USART0_RX_vect
#endif

ISR(USART_RX_vect)
{
   while( g_uart_inbuf[g_uart_inbuf_head] ) {
      if ( (UCSR0A & ((1<<RXC0)|(1<<FE0)|(1<<DOR0)|(1<<UPE0))) == (1<<RXC0) ) {
	 g_uart_inbuf[g_uart_inbuf_head] = UDR0;
	 if( !g_uart_inbuf[g_uart_inbuf_head] ) break;
	 if( ++g_uart_inbuf_head >= UART_INBUF_SIZE )
	    g_uart_inbuf_head = 0;
	 g_uart_inbuf[g_uart_inbuf_head] = 0xff;
      } else {
	 break;
      }
   }
}

void uart_init()
{
   uint8_t ucsr0c = 0;

   g_uart_inbuf_head = 0;
   g_uart_inbuf[0] = 0xff;
   
   set_sleep_mode(SLEEP_MODE_IDLE);   
   
   {
      uint16_t ubrr = UART_UBRR;
      
      UCSR0A = 0;
      
      if( 100*(UART_APPROX_BAUD_RATE - UART_BAUD_RATE)/UART_BAUD_RATE >= 5 ) {
	 ubrr *= 2;
	 UCSR0A = (1 << U2X0);	 
      }
      
      UBRR0H = (ubrr >> 8) & 0xff;
      UBRR0L = ubrr & 0xff;
   }

#if UART_PARITY == 1
   ucsr0c |= ((1 << UPM01) | (1 << UPM00)); /* Odd parity */
#elif UART_PARITY == 0
   ucsr0c |= (1 << UPM00);                  /* Even parity */
#endif /* UART_PARITY */
   
  /* 1 or 2 stop bits */

#if UART_STOPBITS == 2
   ucsr0c |= (1 << USBS0);                  /* Two stop bits */
#endif /* UART_STOPBITS */


#if UART_DATABITS == 6
   ucsr0c |= (1 << UCSZ00);
#elif UART_DATABITS == 7
   ucsr0c |= (1 << UCSZ01);
#elif UART_DATABITS == 8
   ucsr0c |= ((1 << UCSZ00) | (1 << UCSZ01));
#endif
  
  UCSR0C = ucsr0c;
  UCSR0B = (1 << RXEN0)|(1 << TXEN0)|(1 << RXCIE0);
}

uint8_t uart_recv()
{
   while(g_uart_inbuf[g_uart_inbuf_head]) {
      sleep_mode();
   }

   return g_uart_inbuf_head;
}

void uart_continue()
{
   g_uart_inbuf[0] = 0xff;
   g_uart_inbuf_head = 0;
}

void *uart_outbuf()
{
   return g_uart_outbuf;
}

void *uart_inbuf()
{
   return g_uart_inbuf;
}

void uart_send()
{
   int i = 0;

   for( i = 0; g_uart_outbuf[i] && i < sizeof(g_uart_outbuf); i++ ) {
      while ((UCSR0A & (1 << UDRE0)) == 0);
	 UDR0 = g_uart_outbuf[i];
   }
}
