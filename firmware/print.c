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

typedef struct _tty_coord {
   uint8_t x;
   uint8_t y;
} tty_coord_t;

static int hd44780_putchar(char c, FILE *stream);

static FILE hd44780_out = FDEV_SETUP_STREAM(hd44780_putchar, NULL,
					    _FDEV_SETUP_WRITE);
static tty_state_t tty_state = { .skip_til_nl = 0 };

static tty_coord_t addr2coord(uint8_t addr)
{
   uint8_t height = eeprom_read_byte(&tty_height);
   uint8_t i;
   tty_coord_t coord = { .x = 0, .y = 0 };

   for( i = 0; i < height; i++ ) {
      uint8_t begin = eeprom_read_byte(&tty_linemap[i*2]);
      uint8_t end = eeprom_read_byte(&tty_linemap[i*2+1]);
      if(addr >= begin && addr < end) {
	 coord.x = addr - begin;
	 coord.y = i;
	 return coord;
      }
   }
   
   /* if we got there we should probably wrap up */
   return coord;
}

static uint8_t coord2addr(tty_coord_t coord)
{
   uint8_t height = eeprom_read_byte(&tty_height);
   uint8_t begin, end;

   if( coord.y >= height )
      coord.y = height-1;
   begin = eeprom_read_byte(&tty_linemap[coord.y*2]);
   end = eeprom_read_byte(&tty_linemap[coord.y*2+1]);
   if(coord.x >= end-begin)
      coord.x = end-1;
   return (begin+coord.x);
}

static tty_coord_t coord_add(tty_coord_t coord, int8_t dx, int8_t dy)
{
   uint8_t height = eeprom_read_byte(&tty_height), width;
   uint8_t begin, end;

   coord.x += dx;
   coord.y += dy;

   while(coord.y >= height)
      coord.y -= height;
   while(coord.y < 0)
      coord.y += height;

   begin = eeprom_read_byte(&tty_linemap[coord.y*2]);
   end = eeprom_read_byte(&tty_linemap[coord.y*2+1]);
   width = end-begin;
   
   while(coord.x >= width)
      coord.x -= width;
   while(coord.x < 0)
      coord.x += width;
  
   return coord;
}

static tty_coord_t line_edge(tty_coord_t coord, int dest)
{
   uint8_t begin, end;

   begin = eeprom_read_byte(&tty_linemap[coord.y*2]);
   end = eeprom_read_byte(&tty_linemap[coord.y*2+1]);

   if(dest)
      coord.x = 0;
   else
      coord.x = end-begin-1;
   return coord;
}

static void goto_addr(uint8_t addr)
{
   hd44780_ir_write(HD44780_CMD_SET_DDRAM_ADDR | addr);
   hd44780_wait_busy();
}

static void move_line_and_home(tty_coord_t coord, int8_t dy)
{
   goto_addr(coord2addr(line_edge(coord_add(coord, 0, dy), 1)));
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
   tty_coord_t pcoord, ncoord;
   tty_policy_t policy = { .data = eeprom_read_byte(&tty_policy) };
   
   paddr = hd44780_wait_busy();
   pcoord = addr2coord(paddr);

   switch(c) {
      case '\n':
	 tty_state.skip_til_nl = 0;
	 move_line_and_home(pcoord, 1);
	 return 0;
      case '\f':
	 clear_screen();
	 return 0;
      case '\b': {
	 tty_coord_t bcoord = coord_add(pcoord, -1, 0);
	 goto_addr(coord2addr(bcoord));
	 hd44780_dr_write(' ');
	 hd44780_wait_busy();
	 goto_addr(coord2addr(bcoord));
	 return 0;
      }
   }

   if( tty_state.skip_til_nl )
      return 0;
      
   hd44780_dr_write(c);
   naddr = hd44780_wait_busy();
   ncoord = addr2coord(naddr);
   
   if( pcoord.y != ncoord.y ) {
      if( policy.conf.eol_expand_line ) {
	 move_line_and_home(pcoord, 1);
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

