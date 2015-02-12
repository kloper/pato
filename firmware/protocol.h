/** -*- C -*-
 * \file
 *
 * \brief Communication protocol for Pato
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
