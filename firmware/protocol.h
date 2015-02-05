/* -*- C -*-
 *
 * Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
 *
 * protocol.h - communication protocol for Pato
 *
 */

#ifndef _pato_protocol_h_
#define _pato_protocol_h_

#define PATO_CMD_PING          0x00
#define PATO_CMD_DIRECT        0x01
#define PATO_CMD_RESET         0x02
#define PATO_CMD_CONTRAST      0x03
#define PATO_CMD_BACKLIGHT     0x04
#define PATO_CMD_PRINT_SETADDR 0x05
#define PATO_CMD_PRINT_GETADDR 0x06
#define PATO_CMD_PRINT_PUT     0x07
#define PATO_CMD_PRINT_COMMIT  0x08

#define PATO_DIRECT_CLR       0x01
#define PATO_DIRECT_HOME      0x02
#define PATO_DIRECT_EMS       0x03
#define PATO_DIRECT_DCTRL     0x04
#define PATO_DIRECT_SHIFT     0x05
#define PATO_DIRECT_FUNC      0x06
#define PATO_DIRECT_CGADDR    0x07
#define PATO_DIRECT_DDADDR    0x08
#define PATO_DIRECT_BUSY_WAIT 0x09
#define PATO_DIRECT_WRITE     0x0A
#define PATO_DIRECT_READ      0x0B

#define PATO_REPLY_OK    0x00
#define PATO_REPLY_ERROR 0xff

#define PATO_ERROR_CRC    0x01
#define PATO_ERROR_BADCMD 0x02
#define PATO_ERROR_BADARG 0x03
#define PATO_ERROR_SEP    0x04

typedef struct _packet {
   uint8_t cmd;
   uint8_t arg0;
   uint8_t arg1;
   uint8_t crc;
   uint8_t zero;
} packet_t;

#endif /* _pato_protocol_h_ */
