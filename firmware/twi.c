/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * twi.c - TWI slave driver
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
#include "twi.h"

#if defined(HAVE_TWI)

typedef struct _twi_state {
   uint8_t index;
   uint8_t data[sizeof(packet_t)];
   uint8_t status;
} twi_state_t;


twi_state_t g_twi_input = {
   .index = 0,
   .data = {0, 0, 0, 0, 0},
   .status = 0xff
};

twi_state_t g_twi_output = {
   .index = 0,
   .data = {0, 0, 0, 0, 0},
   .status = 0xff
};

ISR(TWI_vect)
{
volatile uint8_t twsr, twcr;
   twsr = TWSR & 0xF8;
   
   twcr = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
   
   switch(twsr) {
      case 0xA8:
      case 0xB0:
	    g_twi_output.index = 0; 
      case 0xB8:
	 if(g_twi_output.index < sizeof(packet_t))
	    TWDR = g_twi_output.data[g_twi_output.index++];
	 else
	    TWDR = 0;
	 if(g_twi_output.index < sizeof(packet_t))
	    twcr |= (1<<TWEA);
	 break;
      case 0x60:	 
      case 0x68:
	 g_twi_input.index = 0;
	 twcr |= (1<<TWEA);
	 break;
      case 0x80:
	 if(g_twi_input.index < sizeof(packet_t))
	    g_twi_input.data[g_twi_input.index++] = TWDR;
	 if(g_twi_input.index < sizeof(packet_t))
	    twcr |= (1<<TWEA);
	 break;
      case 0:
	 twcr |= (1<<TWEA) | (1<<TWSTO);
      default:
	 twcr |= (1<<TWEA);
	 break;
   }

   TWCR = twcr;
}

void twi_init()
{
   uint32_t baudrate = eeprom_read_dword(&twi_baudrate);
   uint32_t twbr = (F_CPU / baudrate - 16) / 2;
   uint8_t twsr = 0;
   uint8_t twar = eeprom_read_byte(&twi_slaveaddr);
   
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
   uint8_t index = 0;

   do {
      cli();
      index = state->index;
      sei();

      if(index >= sizeof(state->data))
	 break;
	 
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
}

packet_t *twi_slave_recv()
{
   cli();
   g_twi_input.status = 0;
   g_twi_input.index = 0;
   sei();

   TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);

   twi_slave_wait(&g_twi_input);

   return (packet_t*)g_twi_input.data;
}

void *twi_outbuf()
{
   return g_twi_output.data;
}

#endif /* HAVE_TWI */
