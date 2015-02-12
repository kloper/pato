/** -*- C -*-
 * \file
 *
 * \brief TWI side of the TWI-I2C bridge
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

#ifndef _pato_bridge_twi_h_
#define _pato_bridge_twi_h_

typedef struct _twi_packet_header {
   uint8_t rw:1;
   uint8_t addr:7;  
} twi_packet_header_t;

typedef struct _twi_config {
   uint16_t twps:2;
   uint16_t twbr:8;
} twi_config_t;

typedef struct _twi_wait_result {
   uint8_t status;
   uint8_t remaining;
} twi_wait_result_t;

extern void twi_init(void);
extern void twi_config(twi_config_t config);
extern void twi_master_send(twi_packet_header_t *data, uint8_t size,
			    uint8_t stop);
extern void twi_master_recv(twi_packet_header_t *data, uint8_t size,
			    uint8_t stop, uint8_t last_nack);
extern twi_wait_result_t twi_master_wait(void);

#endif /* _pato_bridge_twi_h_ */
