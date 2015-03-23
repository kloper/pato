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

   typedef enum {
      USER_DEFINED = 1,
      SHUTDOWN = 2,
      DEFAULT = 0
   } reset_type;
   
   Pato(const AbstractTransport &transport):
      m_transport(transport),
      m_format_cache(NULL)
   {}

   uint8_t ping(uint8_t val) {
      if( query(PATO_CMD_PING, 0, val) )
         return m_reply.arg1;
      return 0xff;
   }

   uint8_t clear_screen() {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_CLR, 0) )
         return m_reply.arg0;
      return 0xff;
   }

   uint8_t return_home() {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_HOME, 0) )
         return m_reply.arg0;
      return 0xff;
   }

   uint8_t entry_mode_set(const bool shift_direction,
                          const bool shift_subject) {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_EMS,
                (shift_direction ? 0x02 : 0) | (shift_subject ? 0x01 : 0)) )
         return m_reply.arg0;
      return 0xff;
   }

   uint8_t display_control(const bool display_on,
                           const bool cursor_on,
                           const bool cursor_blink) {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_DCTRL,
                (display_on ? 0x04 : 0) |
                (cursor_on ? 0x02 : 0)  |
                (cursor_blink ? 0x01 : 0)) )
         return m_reply.arg0;
      return 0xff;      
   }

   uint8_t shift_control(const bool display_shift,
                 const bool right_shift) {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_SHIFT,
                (display_shift ? 0x08 : 0) |
                (right_shift ? 0x04 : 0)) )
         return m_reply.arg0;
      return 0xff;      
   }

   uint8_t set_cgram_addr(const uint8_t addr) {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_CGADDR, addr & 0x3f) )
         return m_reply.arg0;
      return 0xff;      
   }

   uint8_t set_ddram_addr(const uint8_t addr) {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_DDADDR, addr & 0x7f) )
         return m_reply.arg0;
      return 0xff;      
   }

   uint8_t busy_wait() {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_BUSY_WAIT, 0) )
         return m_reply.arg0;
      return 0xff;      
   }
   
   uint8_t display_write(const uint8_t data) {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_WRITE, data) )
         return m_reply.arg0;
      return 0xff;      
   }

   uint8_t display_read(uint8_t &data) {
      if( query(PATO_CMD_DIRECT, PATO_DIRECT_READ, 0) ) {
         data = m_reply.arg1;
         return m_reply.arg0;
      }
      return 0xff;      
   }

   uint8_t reset_display(const reset_type type = DEFAULT,
                        const uint8_t data = 0) {
      if( query(PATO_CMD_RESET, type, data) ) {
         return m_reply.arg0;
      }
      return 0xff;      
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
