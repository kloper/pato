/** -*- C -*-
 * \file
 *
 * \brief Pato main
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

#include <string.h>
#include <stdint.h>

#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "config.h"
#include "protocol.h"
#include "hd44780.h"
#include "uart.h"
#include "twi.h"
#include "crc.h"
#include "print.h"

#if defined(HAVE_UART)
#define comm_init uart_init
#define comm_outbuf uart_outbuf
#define comm_recv uart_recv
#define comm_send uart_send
#define comm_skip()
#elif defined(HAVE_TWI)
#define comm_init twi_init
#define comm_outbuf twi_outbuf
#define comm_recv twi_slave_recv
#define comm_send twi_slave_send
#define comm_skip twi_slave_skip
#else
#error "No communication interface has been configured"
#endif

#if defined(DEBUG)
#define INCR_COUNTER_RECV g_counters.recv_count++
#define INCR_COUNTER_SEND g_counters.send_count++
#define INCR_ERROR_SEP    g_counters.error_sep_count++
#define INCR_ERROR_CRC    g_counters.error_crc_count++
#define INCR_ERROR_OTHER  g_counters.error_other_count++

typedef struct _counters {
   uint32_t recv_count;
   uint32_t send_count;
   uint32_t error_sep_count;
   uint32_t error_crc_count;
   uint32_t error_other_count;
} counters_t;

counters_t g_counters = {
   .recv_count = 0,
   .send_count = 0,
   .error_sep_count = 0,
   .error_crc_count = 0,
   .error_other_count = 0
};
#else
#define INCR_COUNTER_RECV 
#define INCR_COUNTER_SEND 
#define INCR_ERROR_SEP    
#define INCR_ERROR_CRC    
#define INCR_ERROR_OTHER  
#endif /* DEBUG */

int main(int argc, char *argv[])
{
   packet_t *packet = NULL, *reply = NULL;
   uint8_t reset_value = 0, cmd = 0;
   
   comm_init();

   reset_value = eeprom_read_byte(&hd44780_initfunc);

   hd44780_reset(reset_value);
   
   hd44780_ir_write(HD44780_CMD_DISPLAY         |
		    HD44780_CMD_DISPLAY_ON      |
		    HD44780_CMD_DISPLAY_CURS_ON |
		    HD44780_CMD_DISPLAY_CURS_BLINK );
   hd44780_wait_busy();
   
   hd44780_ir_write(HD44780_CMD_EMS |
		    HD44780_CMD_EMS_INCR);
   hd44780_wait_busy();

   sei();

#if defined(HAVE_PRINT)
   hd44780_print_reset();
#endif /* HAVE_PRINT */
   
   reply = (packet_t*)comm_outbuf();

   while(1) {
      packet = comm_recv();
      INCR_COUNTER_RECV;
      
      if( packet->zero != 0 ) {
	 reply->cmd = PATO_REPLY_ERROR;
	 reply->arg0 = PATO_ERROR_SEP;
	 reply->arg1 = packet->cmd;
         INCR_ERROR_SEP;
      } else if(packet->crc != crc8( (uint8_t*)packet, sizeof(packet_t) - 2)) {
	 reply->cmd = PATO_REPLY_ERROR;
	 reply->arg0 = PATO_ERROR_CRC;
	 reply->arg1 = packet->cmd;
         INCR_ERROR_CRC;
      } else {
	 switch(packet->cmd) {
	    case PATO_CMD_PING:
	       reply->cmd = PATO_REPLY_OK;
	       reply->arg0 = packet->cmd;
	       reply->arg1 = packet->arg1 + 1;
	       break;
#if defined(HAVE_DIRECT_API)
	    case PATO_CMD_DIRECT:
	       switch(packet->arg0) {
		  case PATO_DIRECT_CLR:
		     cmd = HD44780_CMD_CLEAR_SCREEN;
		     break;
		  case PATO_DIRECT_HOME:
		     cmd = HD44780_CMD_RETURN_HOME;
		     break;
		  case PATO_DIRECT_EMS:
		     cmd = HD44780_CMD_EMS | (packet->arg1 & 0x3);
		     break;
		  case PATO_DIRECT_DCTRL:
		     cmd = HD44780_CMD_DISPLAY | (packet->arg1 & 0x7);
		     break;
		  case PATO_DIRECT_SHIFT:
		     cmd = HD44780_CMD_SHIFT | (packet->arg1 & 0xf);
		     break;
		  case PATO_DIRECT_FUNC:
		     cmd = HD44780_CMD_FUNC_SET | (packet->arg1 & 0x1f);
		     break;
		  case PATO_DIRECT_CGADDR:
		     cmd = HD44780_CMD_SET_CGRAM_ADDR | (packet->arg1 & 0x3f);
		     break;
		  case PATO_DIRECT_DDADDR:
		     cmd = HD44780_CMD_SET_DDRAM_ADDR | (packet->arg1 & 0x7f);
		     break;
		  case PATO_DIRECT_BUSY_WAIT:
		     break;
		  case PATO_DIRECT_WRITE:
		     hd44780_dr_write(packet->arg1);
		     break;
		  case PATO_DIRECT_READ:
		     reply->arg1 = hd44780_dr_read();
		     break;
		  default:
		     cmd = 0xff;
		     break;
	       }

	       if(cmd != 0 && cmd != 0xff) {
		  hd44780_ir_write(cmd);
	       }
	       if(cmd != 0xff) {
		  reply->cmd = PATO_REPLY_OK;
		  reply->arg0 = hd44780_wait_busy();
	       } else {
		  reply->cmd = PATO_REPLY_ERROR;
		  reply->arg0 = PATO_ERROR_BADARG;
		  reply->arg1 = packet->cmd;
                  INCR_ERROR_OTHER;
	       }
	       break;
	    case PATO_CMD_RESET:
	       switch(packet->arg0) {
		  case 1:
		     cmd = packet->arg1;
		     break;
		  case 2:
		     cmd = 0xff;
		     break;
		  default:
		  case 0:
		     cmd = reset_value;
		     break;
	       }
               cli();
	       hd44780_reset(cmd);
               sei();

#if defined(HAVE_PRINT)
               hd44780_print_reset();
#endif /* HAVE_PRINT */
               
	       reply->cmd = PATO_REPLY_OK;
	       reply->arg0 = cmd;
	       reply->arg1 = packet->cmd;
	       break;
#endif /* HAVE_DIRECT_API */               
#if defined(HAVE_PRINT)               
	    case PATO_CMD_PRINT_SETADDR:
	       cmd = hd44780_print_set_addr(*(uint16_t*)&packet->arg0);
	       reply->cmd = PATO_REPLY_OK;
	       reply->arg0 = cmd;
	       reply->arg1 = packet->cmd;
	       break;
	    case PATO_CMD_PRINT_GETADDR: {
	       *(uint16_t*)&reply->arg0 = hd44780_print_get_addr();
	       reply->cmd = PATO_REPLY_OK;
	    } break;
	    case PATO_CMD_PRINT_COMMIT:
	       cmd = hd44780_print_commit(*(uint16_t*)&packet->arg0);
	       reply->cmd = PATO_REPLY_OK;
	       reply->arg0 = cmd;
	       reply->arg1 = packet->cmd;
	       break;
	    case PATO_CMD_PRINT_PUT:
	       cmd = hd44780_print_put(packet->arg0, packet->arg1);
	       reply->cmd = PATO_REPLY_OK;
	       reply->arg0 = cmd;
	       reply->arg1 = packet->cmd;
	       break;
	    case PATO_CMD_PRINT_PUT_PTR:
	       cmd = hd44780_print_put_ptr(*(uint16_t*)&packet->arg0);
	       reply->cmd = PATO_REPLY_OK;
	       reply->arg0 = cmd;
	       reply->arg1 = packet->cmd;
	       break;
#endif /* HAVE_PRINT */          
	    default:
	       reply->cmd = PATO_REPLY_ERROR;
	       reply->arg0 = PATO_ERROR_BADCMD;
	       reply->arg1 = packet->cmd;
               INCR_ERROR_OTHER;
	       break;
	 }
      }

      reply->crc = crc8( (uint8_t*)reply, sizeof(packet_t) - 2);
      reply->zero = 0;


      comm_skip();
      comm_send();
      INCR_COUNTER_SEND;
   }
   return 0;
}
