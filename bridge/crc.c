/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * crc.c - crc16 calculation based on util/crc16.h
 *
 */

#include <stdint.h>

#include <util/crc16.h>

#include "crc.h"

uint16_t crc16(uint8_t *ptr, uint16_t size)
{
   uint16_t crc = 0;
   
   while(size--) {
      crc = _crc16_update(crc, *ptr);
      ptr++;
   }

   return crc;
}
