/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * main.c - UART-I2C bridge main
 *
 */

#include <string.h>
#include <stdint.h>

#include <avr/interrupt.h>

#include "config.h"
#include "protocol.h"
#include "uart.h"
#include "cobs.h"
#include "crc.h"

int main(int argc, char *argv[])
{
   packet_header_t *request = NULL,  *reply = NULL;
   uint8_t *request_last = NULL, *reply_last = NULL,
	   *request_body = NULL, *reply_body = NULL;
   uint8_t size = 0, error_code = 0;
   
   uart_init();

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
	    *(uint32_t*)reply_body = *(uint32_t*)request_body + 1;
	    reply_last = reply_body + sizeof(uint32_t);
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
