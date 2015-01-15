/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * protocol.h - protocol definition for brige communication
 *
 */

#ifndef _pato_bridge_protocol_h_
#define _pato_bridge_protocol_h_

#define PATO_BRIDGE_ERROR 0
#define PATO_BRIDGE_CMD_PING 1

#define PATO_BRIDGE_ERROR_COBS 1
#define PATO_BRIDGE_ERROR_CRC 2

typedef struct _packet_header {
   uint8_t pad;
   uint16_t crc;
   uint8_t cmd;
} packet_header_t;

typedef struct _bridge_error {
   uint8_t request_cmd;
   uint8_t error_code;
} brige_error_t;

#endif /* _pato_bridge_protocol_h_ */
