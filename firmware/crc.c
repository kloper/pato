/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * crc.c - 8-bit CRC calculation based on util/crc16.h
 *
 */

#include <stdint.h>

#include <util/crc16.h>

#include "crc.h"

uint8_t crc8(uint8_t *ptr, uint16_t size)
{
   uint8_t crc = 0;
   
   while(size--) {
      crc = _crc_ibutton_update(crc, *ptr);
      ptr++;
   }

   return crc;
}
