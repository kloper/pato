/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * protocol.h - communication protocol for Pato
 *
 */

#ifndef _pato_protocol_h_
#define _pato_protocol_h_

typedef struct _packet {
   uint8_t cmd;
   uint8_t arg0;
   uint8_t arg1;
   uint8_t crc;
   uint8_t zero;
} packet_t;

#endif /* _pato_protocol_h_ */
