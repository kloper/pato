/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * uart.c - UART driver for communication with host
 *
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
