/** -*- C -*-
 * \file
 *
 * \brief TWI slave driver
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
#include "twi.h"

#if defined(HAVE_TWI)

typedef struct _twi_state {
   uint8_t index;
   uint8_t data[sizeof(packet_t)];
   uint8_t status;
} twi_state_t;


twi_state_t g_twi_input = {
   .index = 5,
   .data = {0, 0, 0, 0, 0},
   .status = 0xff
};

twi_state_t g_twi_output = {
   .index = 5,
   .data = {0, 0, 0, 0, 0},
   .status = 0xff
};

#ifdef DEBUG_TWI
uint8_t g_twi_states[10];
uint8_t g_twi_states_cur = 0;
#endif /* DEBUG_TWI */

ISR(TWI_vect)
{
   volatile uint8_t twsr, twcr;

   twsr = TWSR & 0xF8;   
   twcr = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);

#ifdef DEBUG_TWI
   g_twi_states[g_twi_states_cur++] = twsr;
   if(g_twi_states_cur >= sizeof(g_twi_states))
      g_twi_states_cur = 0;
#endif /* DEBUG_TWI */
  
   switch(twsr) {
      case 0xA8:
      case 0xB0:
      case 0xB8:
	 if( g_twi_output.index < sizeof(packet_t) )
	    TWDR = g_twi_output.data[g_twi_output.index++];
	 else
	    TWDR = 0;
	 twcr |= (1<<TWEA);
	 break;
      case 0xC0:
      case 0xC8:
	 g_twi_output.status = twsr;
	 twcr |= (1<<TWEA);
	 break;
      case 0x60:	 
      case 0x68:
	 if(g_twi_input.index < sizeof(packet_t))
	    twcr |= (1<<TWEA);
	 break;
      case 0x80:
	 if(g_twi_input.index < sizeof(packet_t))
	    g_twi_input.data[g_twi_input.index++] = TWDR;
	 if(g_twi_input.index < sizeof(packet_t))
	    twcr |= (1<<TWEA);
         else {
            g_twi_input.status = twsr;
            twcr &= ~((1<<TWINT)|(1<<TWIE));
         }
	 break;
      case 0x88: 
      case 0xA0:	 
	 g_twi_input.status = twsr;
	 twcr |= (1<<TWEA);	 
	 break;	 
      case 0:
	 twcr |= (1<<TWEA) | (1<<TWSTO);
	 g_twi_input.status = 0xff;
	 g_twi_output.status = 0xff;
	 break;

   }

   TWCR = twcr;
}

void twi_init()
{
   uint32_t baudrate = eeprom_read_dword(&g_pato_config.twi_baudrate);
   uint32_t twbr = (F_CPU / baudrate - 16) / 2;
   uint8_t twsr = 0;
   uint8_t twar = eeprom_read_byte(&g_pato_config.twi_slaveaddr);
   
   while(twbr > 0xff && twsr < 4) {
      twbr /= 2;
      twsr++;
   }
   
   TWAR = twar << 1;   
   TWSR = twsr & 0x3;
   TWBR = twbr & 0xff;
   TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
}

static void twi_slave_wait(twi_state_t *state)
{
   uint8_t status = 0;

   do {
      cli();
      status = state->status;
      sei();

      if(status) break;
	 
      sleep_mode();

   } while(1);
}

void twi_slave_send()
{
   cli();
   g_twi_output.status = 0;
   g_twi_output.index = 0;
   sei();

   TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);

   twi_slave_wait(&g_twi_output);

   TWCR = (1<<TWEN) | (1<<TWIE);
}

packet_t *twi_slave_recv()
{
   cli();
   g_twi_input.status = 0;
   g_twi_input.index = 0;
   g_twi_input.data[sizeof(packet_t)-1] = 0xff;
   sei();

   TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);

   twi_slave_wait(&g_twi_input);

   TWCR = (1<<TWEN) | (1<<TWIE);
   
   return (packet_t*)g_twi_input.data;
}

/**
 * @brief Skip any pending input bytes 
 */
void twi_slave_skip()
{
   if( g_twi_input.status == 0xa0 || g_twi_input.status == 0xff)
      return;
   
   cli();
   g_twi_input.status = 0;
   sei();

   TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);

   twi_slave_wait(&g_twi_input);

   TWCR = (1<<TWEN) | (1<<TWIE);
}

void *twi_outbuf()
{
   return g_twi_output.data;
}

#endif /* HAVE_TWI */
