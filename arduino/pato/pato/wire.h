/** -*- C++ -*-
 * \file
 *
 * \brief I2C transport using Wire library
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

#ifndef _pato_wire_h_
#define _pato_wire_h_

namespace pato {

class WireTransport: public pato::AbstractTransport {
public:
   WireTransport(TwoWire &wire, uint8_t slave_addr):
      m_wire(wire),
      m_slave_addr(slave_addr)
   {}
   
   virtual uint8_t query(const packet_t *request, packet_t *reply) const
   {  
      m_wire.beginTransmission(m_slave_addr);
      uint8_t rc = m_wire.write((uint8_t*)request, sizeof(packet_t));      
      m_wire.endTransmission(0); // don't send stop

      if( rc != sizeof(packet_t) )
         return 0;

      rc = m_wire.requestFrom(m_slave_addr, sizeof(packet_t));
      if( rc != sizeof(packet_t) )
         return 0;

      for(int i = 0; i < sizeof(packet_t); i++)
         ((uint8_t*)reply)[i] = m_wire.read();

      return rc;
   }
   
private:
   TwoWire &m_wire;
   uint8_t m_slave_addr;
};

}; // end of namespace pato

#endif /* _pato_wire_h_ */
