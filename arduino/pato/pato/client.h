/** -*- C++ -*-
 * \file
 *
 * \brief Arduino client for Pato - HD44780 Display Driver based on Atmega
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

#ifndef _pato_client_h_
#define _pato_client_h_

namespace pato {

class Pato {
public:
   typedef enum {
      NO_ERROR = 0,
      TWI_ERROR = 1,
      CRC_ERROR = 2,
      SEP_ERROR = 3,
      REP_ERROR = 4
   } error_t;
   
   Pato(const AbstractTransport &transport):
      m_transport(transport),
      m_format_cache(NULL)
   {}

   uint8_t ping(uint8_t val) {
      if( query(PATO_CMD_PING, 0, val) )
         return m_reply.arg1;
      return 0;
   }
   
protected:
   bool query(uint8_t cmd, uint8_t arg0, uint8_t arg1) {
      m_error = NO_ERROR;
      
      m_request.cmd = cmd;
      m_request.arg0 = arg0;
      m_request.arg1 = arg1;
      m_request.crc = crc8((uint8_t*)&m_request, sizeof(packet_t) - 2);
      m_request.zero = 0;

      uint8_t rc = m_transport.query( &m_request, &m_reply );
      if( rc != sizeof(m_reply) ) {
         m_error = TWI_ERROR;
         return false;
      }
      if( m_reply.zero != 0 ) {
         m_error = SEP_ERROR;
         return false;
      }
      if( m_reply.crc != crc8((uint8_t*)&m_reply, sizeof(packet_t) - 2) ) {
         m_error = CRC_ERROR;
         return false;
      }
      if( m_reply.cmd == PATO_REPLY_ERROR ) {
         m_error = REP_ERROR;
         m_pato_error = m_reply.arg0;
         return false;
      }
      return true;
   }
   
private:
   const uint8_t *m_format_cache;
   packet_t m_request;
   packet_t m_reply;
   error_t m_error;
   uint8_t m_pato_error;
   const AbstractTransport &m_transport;
};

}; // end of namespace pato

#endif /* _pato_client_h_ */
