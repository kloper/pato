/** -*- C -*-
 * \file
 *
 * \brief Protocol definition for brige communication
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

#ifndef _pato_bridge_protocol_h_
#define _pato_bridge_protocol_h_

#define PATO_BRIDGE_ERROR 0
#define PATO_BRIDGE_CMD_PING 1
#define PATO_BRIDGE_CMD_TWI_CONFIG 2
#define PATO_BRIDGE_CMD_TWI_MASTER_SEND 3
#define PATO_BRIDGE_CMD_TWI_MASTER_RECV 4

#define PATO_BRIDGE_ERROR_COBS 1
#define PATO_BRIDGE_ERROR_CRC 2
#define PATO_BRIDGE_ERROR_BADCMD 3
#define PATO_BRIDGE_ERROR_BADARG 4
#define PATO_BRIDGE_ERROR_SIZE 5

typedef struct _packet_header {
   uint8_t pad;
   uint8_t size;
   uint16_t crc;
   uint8_t cmd;
} packet_header_t;

typedef struct _bridge_error {
   uint8_t request_cmd;
   uint8_t error_code;
} brige_error_t;

typedef struct _twi_master_send_args {
   uint8_t stop;
   uint8_t addr;
} twi_master_send_args_t;

typedef struct _twi_master_recv_args {
   uint8_t stop;
   uint8_t last_nack;
   uint8_t addr;
   uint8_t size;
} twi_master_recv_args_t;

#endif /* _pato_bridge_protocol_h_ */
