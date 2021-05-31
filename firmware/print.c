/** -*- C -*-
 * \file
 *
 * \brief Output buffering, delta update and string formatting.
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

#include "config.h"

#if defined(HAVE_PRINT)

#include "hd44780.h"
#include "print.h"

#if defined(HAVE_PRINTF)
#include <stdio.h>
#else
typedef char FILE;
#define FDEV_SETUP_STREAM(a,b,c) 0
#endif

typedef union _tty_policy {
   uint8_t data;
   struct {
      uint8_t eol_expand_line: 1;
      uint8_t tab_size:3;
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
   uint8_t height = eeprom_read_byte(&g_pato_config.tty_height);
   uint8_t i;
   tty_coord_t coord = { .x = 0, .y = 0 };

   for( i = 0; i < height; i++ ) {
      uint8_t begin = eeprom_read_byte(&g_pato_config.tty_linemap[i*2]);
      uint8_t end = eeprom_read_byte(&g_pato_config.tty_linemap[i*2+1]);
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
   uint8_t height = eeprom_read_byte(&g_pato_config.tty_height);
   uint8_t begin, end;

   if( coord.y >= height )
      coord.y = height-1;
   begin = eeprom_read_byte(&g_pato_config.tty_linemap[coord.y*2]);
   end = eeprom_read_byte(&g_pato_config.tty_linemap[coord.y*2+1]);
   if(coord.x >= end-begin)
      coord.x = end-1;
   return (begin+coord.x);
}

static tty_coord_t coord_add(tty_coord_t coord, int8_t dx, int8_t dy)
{
   uint8_t height = eeprom_read_byte(&g_pato_config.tty_height), width;
   uint8_t begin, end;

   coord.x += dx;
   coord.y += dy;

   while(coord.y >= height)
      coord.y -= height;
   while(coord.y < 0)
      coord.y += height;

   begin = eeprom_read_byte(&g_pato_config.tty_linemap[coord.y*2]);
   end = eeprom_read_byte(&g_pato_config.tty_linemap[coord.y*2+1]);
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

   begin = eeprom_read_byte(&g_pato_config.tty_linemap[coord.y*2]);
   end = eeprom_read_byte(&g_pato_config.tty_linemap[coord.y*2+1]);

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
   tty_policy_t policy = {
      .data = eeprom_read_byte(&g_pato_config.tty_policy)
   };
   
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
#ifndef HAVE_PRINT_MINIMAL
      case '\b': {
	 tty_coord_t bcoord = coord_add(pcoord, -1, 0);
	 goto_addr(coord2addr(bcoord));
	 hd44780_dr_write(' ');
	 hd44780_wait_busy();
	 goto_addr(coord2addr(bcoord));
	 return 0;
      }
      case '\t': {
         if(policy.conf.eol_expand_line)
            goto_addr(coord2addr(coord_add(pcoord, policy.conf.tab_size, 0)));
         else {
            tty_coord_t edge = line_edge(pcoord, 0);
            tty_coord_t ncoord = pcoord;
            ncoord.x = min(pcoord.x + policy.conf.tab_size, edge.x);
            goto_addr(coord2addr(ncoord));
         }
         return 0;
      } 
#endif
      case '\v': {
         goto_addr(coord2addr(coord_add(pcoord, 0, 1)));
         return 0;
      } 
      case '\r': {
         goto_addr(coord2addr(line_edge(pcoord,1)));
         return 0;
      } 
      case '\x1b': {
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

static print_buffer_t print_buffer = { .addr = 0, .buffer = { 0 } };

/**
 * @brief Reset print buffer state.
 *
 * Clear print buffer content and current address and clear the screen.
 */
void hd44780_print_reset()
{
   print_buffer.addr = 0;
   *print_buffer.buffer = 0;
   
   clear_screen();
}

/**
 * @brief Flush print buffer into display, applying formatting (if available)
 *
 * Without printf support, interpret the print buffer just a null terminated 
 * string and copy it to memory.
 *
 * With printf support the buffer shall hold both pintf's format string and
 * its parameters in binary format ready to be consumed by the printf() 
 * function. The format string will be always taken from the beginning of the
 * buffer and must be null terminated.
 * 
 * The input argument defines  uint16_t offset inside the buffer. If the offset
 * is zero, the arguments for printf() are taken immediately after the 
 * null character terminating the format string. Otherwise, the offset is taken
 * as the start of parameters for printf().
 * 
 * Without printf support, the offset is checked for validity but not used.
 *
 * With or without printf support copying to display is performed with 
 * applying terminal characters and escape sequences.
 *
 * @param[in] offset offset in the print buffer to start of printf()'s arguments
 * @returns 
 * True of the format string is zero terminated and the offset is inside of 
 * buffer.
 *
 * @see print_buffer_t
 */
uint8_t hd44780_print_commit(uint16_t offset)
{
   uint8_t *args;

   args = memchr(print_buffer.buffer, 0, PATO_PRINT_BUFFER_SIZE);
   if( args == NULL )
      return 0;

   args += 1;
   
   if( offset > 0 ) {
      if(offset < PATO_PRINT_BUFFER_SIZE ) 
         args = print_buffer.buffer + offset;
      else 
         return 0;
   }
   
#if defined(HAVE_PRINTF)      
   vfprintf(&hd44780_out, (const char*)print_buffer.buffer, args);
#else
   for( uint8_t *ch = print_buffer.buffer; ch < args; ch++ )
      if( hd44780_putchar(*ch, &hd44780_out) )
         break;
#endif /* HAVE_PRINTF */
   return 1;
}

/**
 * @brief Put two bytes at current address at print buffer
 * 
 * This function puts two bytes inside print buffer. Each byte is put
 * at current address and the address is autoincremented. 
 *
 * @param[in] arg0 first byte to be put
 * @param[in] arg1 second byte to be put.
 * @returns 
 * True if the buffer has enough space for both bytes. 
 * If the buffer has none or a single free byte none of the input bytes
 * is written and the current address is not incremented.
 */
uint8_t hd44780_print_put(uint8_t arg0, uint8_t arg1)
{
   if( print_buffer.addr < PATO_PRINT_BUFFER_SIZE-2 ) {
      print_buffer.buffer[print_buffer.addr++] = arg0;
      print_buffer.buffer[print_buffer.addr++] = arg1;
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

#ifndef HAVE_PRINT_MINIMAL

uint16_t hd44780_print_get_addr()
{
	return print_buffer.addr;
}

/**
 * @brief Put `char*` into print buffr
 * 
 * This is a special version of hd44780_print_put(). It will treat incoming
 * argument as uint16_t offset from beginning of the print buffer.
 * It will add the offset to the pointer to the beginning of the print buffer
 * and save the resulting pointer at the current address. 
 *
 * This is required for treating "%s" formats that require both pointer and
 * actual string to be delvered from master.
 *
 * This function increments the current print address by two.
 *
 * @param[in] offset offset from beginning of the print buffer to create 
 *                   argument pointer
 * @returns True if the operation was successfull (valid offset)
 */
uint8_t hd44780_print_put_ptr(uint16_t offset)
{
   if( print_buffer.addr < PATO_PRINT_BUFFER_SIZE-2 &&
       offset >= 0 && offset < PATO_PRINT_BUFFER_SIZE )
   {
      uint8_t *ptr = print_buffer.buffer + offset;
      print_buffer.buffer[print_buffer.addr++] = ((uint16_t)ptr) & 0xff;
      print_buffer.buffer[print_buffer.addr++] = ((uint16_t)ptr >> 8) & 0xff;
      return 1;
   }

   return 0;
}

#endif // HAVE_PRINT_MINIMAL

#endif /* HAVE_PRINT */
