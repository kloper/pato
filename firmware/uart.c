/** -*- C -*-
 * \file
 *
 * \brief UART driver for communication with host
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

#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "config.h"
#include "protocol.h"
#include "uart.h"
#include "timeout.h"

#if defined(HAVE_UART)

uint8_t g_uart_inbuf_head = 0;
uint8_t g_uart_inbuf[UART_INBUF_SIZE];
uint8_t g_uart_outbuf[UART_OUTBUF_SIZE];

#ifdef UART_ISR_C_IMPL
ISR(USART0_RX_vect)
{
   uint8_t ucsr = 0;
   uint8_t udr = 0;
   
   g_uart_inbuf[UART_INBUF_SIZE-1] = 0xff;

   do {
      ucsr = UCSR0A;
      udr = UDR0;
      if ( (ucsr & ((1<<RXC0)|(1<<FE0)|(1<<DOR0)|(1<<UPE0))) == (1<<RXC0) ) {
	 g_uart_inbuf[g_uart_inbuf_head] = udr;
	 g_uart_inbuf_head = (g_uart_inbuf_head+1) % UART_INBUF_SIZE;
      } else {
	 break;
      }
   } while(1);
}
#endif /* UART_ISR_C_IMPL */

void uart_init()
{
   uint8_t ucsr0c = 0;

   g_uart_inbuf[UART_INBUF_SIZE-1] = 0xff;
   
   set_sleep_mode(SLEEP_MODE_IDLE);   
   
   {
      uint32_t baudrate = eeprom_read_dword(&uart_baudrate);
      uint32_t ubrr = F_CPU/16/baudrate-1;
      uint32_t approx_baudrate = F_CPU/16/(ubrr+1);

      UCSR0A = 0;
      
      if( 100*(approx_baudrate - baudrate)/baudrate >= 5 ) {
	 ubrr *= 2;
	 UCSR0A = (1 << U2X0);	 
      }
      
      UBRR0H = (ubrr >> 8) & 0xff;
      UBRR0L = ubrr & 0xff;
   }
   
  /* Select parity */

  {
     uint8_t parity = eeprom_read_byte(&uart_parity);

     switch(parity) {
	case 1:
	   ucsr0c |= ((1 << UPM01) | (1 << UPM00)); /* Odd parity */
	   break;
	case 0:
	   ucsr0c |= (1 << UPM00);                  /* Even parity */
	   break;
     }
  }
  
  /* 1 or 2 stop bits */

  {
     uint8_t stopbits = eeprom_read_byte(&uart_stopbits);

     if( stopbits == 2 ) {
	ucsr0c |= (1 << USBS0);                  /* Two stop bits */
     }
  }


  /* Word size */
  {
     uint8_t databits = eeprom_read_byte(&uart_databits);
     switch(databits) {
	case 5:
	   break;
	case 6:
	   ucsr0c |= (1 << UCSZ00);
	   break;
	case 7:
	   ucsr0c |= (1 << UCSZ01);
	   break;
	case 8:
	default:
	   ucsr0c |= ((1 << UCSZ00) | (1 << UCSZ01));
	   break;
     }
  }
  
  UCSR0C = ucsr0c;
  UCSR0B = (1 << RXEN0)|(1 << TXEN0)|(1 << RXCIE0);

  DDRE  |= (1 << 3); 
  PORTE |= (1 << 2); 
}

packet_t *uart_recv()
{
   packet_t *packet = (packet_t*)((uint8_t*)g_uart_inbuf);
   
   while(packet->zero) {
      sleep_mode();
   }

   packet->zero = 0xff;
   
   return packet;
}

void *uart_outbuf()
{
   return g_uart_outbuf;
}

void uart_send()
{
   int i = 0;

   for( i = 0; i < sizeof(g_uart_outbuf); i++ ) {
      while ((UCSR0A & (1 << UDRE0)) == 0);
      UDR0 = g_uart_outbuf[i];
   }
}

#endif /* HAVE_UART */
