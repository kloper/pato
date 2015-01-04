/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * main.c - CCM2040 driver test program
 *
 */

#include <string.h>
#include <stdint.h>

#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "config.h"
#include "protocol.h"
#include "hd44780.h"
#include "uart.h"
#include "crc.h"

int main(int argc, char *argv[])
{
   packet_t *packet = NULL, *reply = NULL;
   uint8_t reset_value = 0, cmd = 0;
   
   uart_init();

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

   reply = (packet_t*)uart_outbuf();
   
   while(1) {
      packet = uart_recv();

      if( packet->crc != crc8( (uint8_t*)packet, sizeof(packet_t) - 2) ) {
	 reply->cmd = PATO_REPLY_ERROR;
	 reply->arg0 = PATO_ERROR_CRC;
	 reply->arg1 = packet->cmd;
      } else {
	 switch(packet->cmd) {
	    case PATO_CMD_PING:
	       reply->cmd = PATO_REPLY_OK;
	       reply->arg0 = packet->cmd;
	       reply->arg1 = packet->arg1 + 1;
	       break;
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
	       hd44780_reset(cmd);
	       reply->cmd = PATO_REPLY_OK;
	       reply->arg0 = cmd;
	       reply->arg1 = packet->cmd;
	       break;
	    default:
	       reply->cmd = PATO_REPLY_ERROR;
	       reply->arg0 = PATO_ERROR_BADCMD;
	       reply->arg1 = packet->cmd;
	       break;
	 }
      }

      reply->crc = crc8( (uint8_t*)reply, sizeof(packet_t) - 2);
      reply->zero = 0;

      uart_send();

   }
   return 0;
}
