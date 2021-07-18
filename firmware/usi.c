/** -*- C -*-
 * \file
 *
 * \brief USI slave driver
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
#include "usi.h"

#if defined(HAVE_USI)

#if defined(__AVR_ATtiny2313__)
#define USI_PORT PORTB
#define USI_DDR DDRB
#define USI_OUT PORTB
#define USI_IN PINB
#define USI_SDA PB5
#define USI_SCL PB7
#endif

typedef enum _usi_state_enum {
   USI_STATE_INIT,
   USI_STATE_ADDR,
   USI_STATE_SEND,
   USI_STATE_SEND_ACK,
   USI_STATE_SEND_FIN,
   USI_STATE_RECV,
   USI_STATE_RECV_ACK
} usi_state_enum_t;

typedef enum _use_mode_enum {
  USI_MODE_WAIT,
  USI_MODE_RECV,
  USI_MODE_SEND,
} usi_mode_enum_t;

typedef struct _usi_state {
  uint8_t addr;
  uint8_t index;
  uint8_t data[sizeof(packet_t)];
  usi_state_enum_t state;
  usi_mode_enum_t mode;
} usi_state_t;


usi_state_t g_usi = {
   .addr = 0xff,
   .index = 5,
   .data = {0x0, 0x0, 0x0, 0x0, 0x0},
   .state = USI_STATE_INIT,
   .mode = USI_MODE_WAIT
};

#ifdef DEBUG_USI
uint8_t g_usi_states[10] = {0};
uint8_t g_usi_states_cur = 0;
#endif /* DEBUG_USI */

ISR(USI_START_vect)
{
   g_usi.index = 0;
   g_usi.state = USI_STATE_ADDR;
   
   USI_DDR &= ~(1<<USI_SDA);
   USICR = (1<<USISIE)|(1<<USIOIE)|(1<<USIWM1)|(1<<USICS1);
   USISR = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC);
}

ISR(USI_OVERFLOW_vect)
{   
   USI_DDR |= (1<<USI_SCL);

#if DEBUG_USI
   g_usi_states[g_usi_states_cur++] = g_usi.state;
   if( g_usi_states_cur >= sizeof(g_usi_states) )
      g_usi_states_cur = 0;
#endif /* DEBUG_USI */
   
   switch(g_usi.state) {
      case USI_STATE_ADDR: {
         volatile uint8_t usidr = USIDR;
         
         if( (usidr >> 1) != g_usi.addr )
            goto reset_label;
         
         g_usi.state = (usidr & 1) ? USI_STATE_SEND : USI_STATE_RECV;

         USIDR = 0;      
         
         if( (g_usi.state == USI_STATE_SEND && g_usi.mode == USI_MODE_SEND) ||
             (g_usi.state == USI_STATE_RECV && g_usi.mode == USI_MODE_RECV) ) {
           // Send ACK
           USI_DDR |= (1<<USI_SDA);
           USI_OUT |= (1<<USI_SDA);       
         } else {
           // Send NACK
         }
         
         USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|
                 (1<<USICNT3)|(1<<USICNT2)|(1<<USICNT1);
      } break;
         
      case USI_STATE_RECV: {
         g_usi.state = USI_STATE_RECV_ACK;
         USI_DDR &= ~(1<<USI_SDA);
         USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|(1<<USICNT0);
      } break;

      case USI_STATE_RECV_ACK: {
         if( g_usi.index >= sizeof(g_usi.data) )
           goto reset_label;
         
         g_usi.state = USI_STATE_RECV;
         g_usi.data[g_usi.index] = USIDR;
         g_usi.index++;
				  
         // Send ACK
         USIDR = 0;                        
         USI_DDR |= (1<<USI_SDA);
		 USI_OUT |= (1<<USI_SDA);
         USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|
                 (1<<USICNT3)|(1<<USICNT2)|(1<<USICNT1)|(1<<USICNT0);
      } break;

      case USI_STATE_SEND_FIN: {	
        g_usi.index++;
		
        volatile uint8_t usidr = USIDR;
        if( (usidr & 3) != 0 ) // NACK received
          goto reset_label;
       		 
        USIDR = 0;
        USI_DDR |= (1<<USI_SDA);
        USI_OUT |= (1<<USI_SDA);
		 
        // Fall through to USI_STATE_SEND
      }
      case USI_STATE_SEND: {
         if( g_usi.index >= sizeof(g_usi.data) )
            goto reset_label;
		
         g_usi.state = USI_STATE_SEND_ACK;            
        
         USIDR = g_usi.data[g_usi.index];
         USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|(1<<USICNT0);
      } break;

      case USI_STATE_SEND_ACK: {
         g_usi.state = USI_STATE_SEND_FIN;

         // Receive ACK
         USIDR = 0; 
         USI_DDR &= ~(1<<USI_SDA);     
         USISR = (1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|
                 (1<<USICNT3)|(1<<USICNT2)|(1<<USICNT1); 
      } break;

   }

   goto exit_label;
   
  reset_label:
   USI_DDR &= ~(1<<USI_SDA);
   g_usi.state = USI_STATE_INIT;
   
   USICR = (1<<USISIE)|(1<<USIWM1)|(1<<USICS1);
   USISR = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC);            

  exit_label:
   USI_DDR &= ~(1<<USI_SCL);
}

static void usi_slave_wait()
{
   volatile uint8_t index = 0;

   cli();
   g_usi.index = 0;
   sei();

   g_usi.state = USI_STATE_INIT;
   USICR = (1<<USISIE)|(1<<USIWM1)|(1<<USICS1);
   USISR = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC);
   
   do {
      cli();
      index = g_usi.index;
      sei();

      if(index >= sizeof(g_usi.data)) break;
	 

      sleep_mode();

   } while(1);

   USICR = 0;
}

void usi_slave_send()
{
  cli();
  g_usi.mode = USI_MODE_SEND;
  sei();

  usi_slave_wait();  
}

packet_t *usi_slave_recv()
{
  cli();
  g_usi.mode = USI_MODE_RECV;
  sei();
  
  usi_slave_wait();

  cli();
  g_usi.mode = USI_MODE_WAIT;
  sei();
  
  return (packet_t*)g_usi.data;
}

/**
 * @brief Skip any pending input bytes 
 */
void usi_slave_skip()
{
}

void *usi_outbuf()
{
   return g_usi.data;
}

void usi_init()
{
	g_usi.addr = eeprom_read_byte(&g_pato_config.twi_slaveaddr);
	
	USI_DDR &= ~((1<<USI_SDA)|(1<<USI_SCL));
}
#endif /* HAVE_USI */
