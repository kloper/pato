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

typedef union _tty_policy {
   uint8_t data;
   struct {
      uint8_t eol_expand_line: 1;
   } conf;
} tty_policy_t;

typedef struct _tty_state {
   uint8_t skip_til_nl: 1;
} tty_state_t;

static int hd44780_putchar(char c, FILE *stream);

static FILE hd44780_out = FDEV_SETUP_STREAM(hd44780_putchar, NULL,
					    _FDEV_SETUP_WRITE);
static tty_state_t tty_state = { .skip_til_nl = 0 };

static uint8_t addr2line(uint8_t addr)
{
   uint8_t height = eeprom_read_byte(&tty_height);
   uint8_t i;

   for( i = 0; i < height; i++ ) {
      uint8_t begin = eeprom_read_byte(&tty_linemap[i*2]);
      uint8_t end = eeprom_read_byte(&tty_linemap[i*2+1]);
      if(addr >= begin && addr < end)
	 return i;
   }
   
   /* if we got there we should probably wrap up */
   return 0;
}

static uint8_t line2addr(uint8_t line)
{
   line %= eeprom_read_byte(&tty_height);
   return eeprom_read_byte(&tty_linemap[line*2]);
}

static void goto_line(uint8_t line)
{
   hd44780_ir_write(HD44780_CMD_SET_DDRAM_ADDR | line2addr(line));
   hd44780_wait_busy();
}

static void goto_addr(uint8_t addr)
{
   hd44780_ir_write(HD44780_CMD_SET_DDRAM_ADDR | addr);
   hd44780_wait_busy();
}

static void clear_screen()
{
   hd44780_ir_write(HD44780_CMD_CLEAR_SCREEN);
   hd44780_wait_busy();

   tty_state.skip_til_nl = 0;
}

static int hd44780_putchar(char c, FILE *stream)
{
   uint8_t paddr, naddr;
   uint8_t pline, nline;
   tty_policy_t policy = { .data = eeprom_read_byte(&tty_policy) };
   
   paddr = hd44780_wait_busy();
   pline = addr2line(paddr);

   switch(c) {
      case '\n':
	 tty_state.skip_til_nl = 0;
	 goto_line(pline+1);
	 return 0;
      case '\f':
	 clear_screen();
	 return 0;
   }

   if( tty_state.skip_til_nl )
      return 0;
      
   hd44780_dr_write(c);
   naddr = hd44780_wait_busy();
   nline = addr2line(naddr);
   
   if( pline != nline ) {
      if( policy.conf.eol_expand_line ) {
	 goto_line(pline+1);
      } else {
	 tty_state.skip_til_nl = 1;
	 goto_addr(paddr);
      }
   }

   return 0;
}

static print_buffer_t print_buffer = { 0, 0 };

uint8_t hd44780_print_commit()
{
   uint8_t *args;
      
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

