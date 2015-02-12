/** -*- C -*-
 * \file
 *
 * \brief UART-I2C bridge main
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

#include <avr/interrupt.h>

#include "config.h"
#include "protocol.h"
#include "uart.h"
#include "cobs.h"
#include "crc.h"
#include "twi.h"

int main(int argc, char *argv[])
{
   packet_header_t *request = NULL,  *reply = NULL;
   uint8_t *request_last = NULL, *reply_last = NULL,
	   *request_body = NULL, *reply_body = NULL;
   uint8_t size = 0, error_code = 0;
   
   uart_init();
   twi_init();
   
   request = (packet_header_t*)uart_inbuf();
   reply = (packet_header_t*)uart_outbuf();
   request_body = (uint8_t*)request + sizeof(packet_header_t);
   reply_body = (uint8_t*)reply + sizeof(packet_header_t);

   sei();
   
   while(1) {
      error_code = 0;
      
      size = uart_recv();
      
      request_last = cobs_decode((uint8_t*)request, size);
      if( !request_last ) {
	 error_code = PATO_BRIDGE_ERROR_COBS;
	 goto error_label;
      }

      if( request->size != request_last - (uint8_t*)&request->cmd ) {
	 error_code = PATO_BRIDGE_ERROR_SIZE;
	 goto error_label;
      }
      
      if( request->crc != crc16((uint8_t*)&request->cmd, request->size ) ) {
	 error_code = PATO_BRIDGE_ERROR_CRC;
	 goto error_label;
      } 

      reply->cmd = request->cmd;
      
      switch(request->cmd) {
	 case PATO_BRIDGE_CMD_PING: {
	    if(request->size - sizeof(request->cmd) != sizeof(uint32_t)) {
	       error_code = PATO_BRIDGE_ERROR_SIZE;
	       goto error_label;
	    }
	    
	    *(uint32_t*)reply_body = *(uint32_t*)request_body + 1;
	    reply_last = reply_body + sizeof(uint32_t);
	 } break;
	 case PATO_BRIDGE_CMD_TWI_CONFIG: {
	    if(request->size - sizeof(request->cmd) != sizeof(twi_config_t)) {
	       error_code = PATO_BRIDGE_ERROR_SIZE;
	       goto error_label;
	    }

	    twi_config(*(twi_config_t*)request_body);
	    twi_init();

	    reply_last = reply_body;
	 } break;
	 case PATO_BRIDGE_CMD_TWI_MASTER_SEND: {
	    twi_master_send_args_t *args =
	       (twi_master_send_args_t*)request_body;
	    twi_wait_result_t *res =
	       (twi_wait_result_t*)reply_body;
	    
	    if(request->size - sizeof(request->cmd) <
	                                     sizeof(twi_master_send_args_t)) {
	       error_code = PATO_BRIDGE_ERROR_SIZE;
	       goto error_label;
	    }

	    args->addr <<= 1;
	    twi_master_send((twi_packet_header_t*)&args->addr,
			    request_last - (uint8_t*)&args->addr,
			    args->stop);

	    *res = twi_master_wait();
	    reply_last = reply_body + sizeof(twi_wait_result_t);
	 } break;
	 case PATO_BRIDGE_CMD_TWI_MASTER_RECV: {
	    twi_master_recv_args_t *args =
	       (twi_master_recv_args_t*)request_body;
	    twi_wait_result_t *res =
	       (twi_wait_result_t*)reply_body;
	    uint8_t *data = reply_body + sizeof(twi_wait_result_t) - 1;
	    
	    if(request->size - sizeof(request->cmd) <
	                                     sizeof(twi_master_recv_args_t)) {
	       error_code = PATO_BRIDGE_ERROR_SIZE;
	       goto error_label;
	    }

	    data[0] = (args->addr << 1);
	    
	    twi_master_recv((twi_packet_header_t*)data,
			    args->size+1, args->stop, args->last_nack);

	    *res = twi_master_wait();
	    reply_last = data +	1 + args->size - res->remaining;
	 } break;

	 default:
	    error_code = PATO_BRIDGE_ERROR_BADCMD;
	    goto error_label;
      }
      
     error_label:
      if( error_code > 0 ) {
	 brige_error_t *error = (brige_error_t*)reply_body;
	 
	 reply->cmd = PATO_BRIDGE_ERROR;
	 error->request_cmd = request->cmd;
	 error->error_code = error_code;
	 reply_last = reply_body + sizeof(brige_error_t);
      }

      uart_continue();

      reply->size = reply_last - (uint8_t*)&reply->cmd;
      reply->crc = crc16((uint8_t*)&reply->cmd, reply->size);
      cobs_encode( (uint8_t*)reply, reply_last - (uint8_t*)reply );
      *reply_last = 0;
      
      uart_send();
   }
   return 0;
}
