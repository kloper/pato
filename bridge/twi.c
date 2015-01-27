/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * twi.c - TWI side of the TWI-I2C bridge
 *
 */

#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "config.h"
#include "twi.h"
#include "wdt.h"

typedef struct _twi_state {
   uint8_t stop:1;
   uint8_t last_nack:1;
   uint8_t count;
   uint8_t *data;
   uint8_t status;
} twi_state_t;


twi_state_t g_twi_state = {
   .stop = 1,
   .last_nack = 1,
   .count = 0,
   .data = NULL,
   .status = 0xff
};

twi_config_t g_twi_config = {
   .twps = 0,
   .twbr = 72
};

ISR(TWI_vect)
{
volatile uint8_t twsr, twcr;
   uint8_t exit = 0;
   
   twsr = TWSR & 0xF8;

   if(g_twi_state.count == 0) {
      twsr = 0xff;
      exit = 1;
      goto exit_label;
   }

   twcr = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
   
   switch(twsr) {
      case 0x8:
      case 0x10:
      case 0x18:
      case 0x28:
	 TWDR = *g_twi_state.data++;
	 g_twi_state.count--;
	 break;
      case 0x50:
      case 0x58:
	 *g_twi_state.data++ = TWDR;
	 g_twi_state.count--;
      case 0x40:	 
	 if(!(g_twi_state.count == 1 && g_twi_state.last_nack))
	    twcr |= (1<<TWEA);
	 if(g_twi_state.count > 0) 
	    break;
      default:
	 exit = 1;
	 goto exit_label;
   }

   TWCR = twcr;

  exit_label:
   if(exit) {
      g_twi_state.status = twsr;
      if(g_twi_state.stop) {
	 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) | (1<<TWIE);
      } else {
	 TWCR = (1<<TWEN);
      }
   }
}

void twi_init()
{
   TWCR = 0;
   TWSR = g_twi_config.twps;
   TWBR = g_twi_config.twbr;
}

void twi_config(twi_config_t config)
{
   g_twi_config.twps = config.twps;
   g_twi_config.twbr = config.twbr;
}
   
void twi_master_send(twi_packet_header_t *data, uint8_t size, uint8_t stop)
{
   data->rw = 0;
   
   cli();

   g_twi_state.stop = stop ? 1 : 0;
   g_twi_state.last_nack = 0;
   g_twi_state.status = 0;
   g_twi_state.count = size;
   g_twi_state.data = (uint8_t*)data;
   
   sei();
   
   TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
}

void twi_master_recv(twi_packet_header_t *data, uint8_t size,
		     uint8_t stop, uint8_t last_nack)
{
   data->rw = 1;
   
   cli();

   g_twi_state.stop = stop ? 1 : 0;
   g_twi_state.last_nack = last_nack ? 1 : 0;
   g_twi_state.status = 0;
   g_twi_state.count = size;
   g_twi_state.data = (uint8_t*)data;

   sei();

   TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
}

twi_wait_result_t twi_master_wait()
{
   twi_wait_result_t rc;
   uint8_t remaining = 0;
   uint8_t retries = 0;
   
   do {
      cli();
      rc.status = g_twi_state.status;
      rc.remaining = g_twi_state.count;
      sei();
      
      if(rc.status) break;
      if(remaining == rc.remaining) {
	 if(retries >= WDT_RETRIES) {
	    if(g_twi_state.stop) {
	       TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO) | (1<<TWIE);
	    } else {
	       TWCR = (1<<TWEN);
	    }	    
	    break;
	 }
      } else {
	 retries = 0;
	 remaining = rc.remaining;
      }

      wdt_enable(WDT_PRESCALE);
      sleep_mode();
      wdt_disable();	    
      retries++;
   } while(1);

   return rc;
}
