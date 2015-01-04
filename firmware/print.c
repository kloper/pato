/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * print.c - printf functionality for Pato
 *
 */

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include <avr/eeprom.h>

#include "config.h"
#include "hd44780.h"
#include "print.h"

static int hd44780_putchar(char c, FILE *stream);

static FILE hd44780_out = FDEV_SETUP_STREAM(hd44780_putchar, NULL,
					    _FDEV_SETUP_WRITE);

static int hd44780_putchar(char c, FILE *stream)
{
   hd44780_dr_write(c);
   hd44780_wait_busy();
   return 0;
}

static print_buffer_t print_buffer = { 0, 0 };

uint8_t hd44780_print_commit()
{
   uint8_t *args;
   
   hd44780_ir_write(HD44780_CMD_CLEAR_SCREEN);
   hd44780_wait_busy();

   args = memchr(print_buffer.buffer, 0, PATO_PRINT_BUFFER_SIZE);
   
   if( args != NULL ) {
      vfprintf(&hd44780_out, (const char*)print_buffer.buffer, args+1);
      return 1;
   }

   return 0;
}

uint8_t hd44780_print_set_addr(uint16_t addr)
{
   if( addr >= 0 && addr < PATO_PRINT_BUFFER_SIZE-2 ) {
      print_buffer.addr = addr;
      return 1;
   }

   return 0;
}

uint16_t hd44780_print_get_addr()
{
   return print_buffer.addr;
}

uint8_t hd44780_print_put(uint8_t arg0, uint8_t arg1)
{
   if( print_buffer.addr < PATO_PRINT_BUFFER_SIZE-2 ) {
      print_buffer.buffer[print_buffer.addr++] = arg0;
      print_buffer.buffer[print_buffer.addr++] = arg1;
      return 1;
   }

   return 0;
}

