/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * cobs.c - Consistent Overhead Byte Stuffing primitives
 *
 * See http://stuartcheshire.org/papers/COBSforToN.pdf
 *
 */

#include <string.h>
#include <stdint.h>

#include "cobs.h"

uint8_t *cobs_encode(uint8_t *buffer, uint8_t size)
{
   uint8_t *code = buffer;

   buffer++;
   size--;
   
   *code = 1;
   while(size--) {
      if( !*buffer ) {
	 code = buffer;
	 *code = 0;
      }
      *code += 1;
      buffer++;
   }

   return buffer;
}

uint8_t *cobs_decode(uint8_t *buffer, uint16_t size)
{
   uint8_t *code = buffer;
   uint8_t *next_code = NULL;

   while( (code - buffer) < size ) {
      if( !*code ) 
	 return NULL;

      next_code = code + *code;
      *code = 0;
      code = next_code;
   }

   return code;
}
