# -*- python -*-
"""@file

@brief I2C-serial transport for pato

Copyright (c) 2014-2015 Dimitry Kloper <kloper@users.sf.net>. 
All rights reserved.

@page License

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation
are those of the authors and should not be interpreted as representing
official policies, either expressed or implied, of the Pato Project.
"""

from __future__ import absolute_import

import serial
import time
import pdb

from util.protocol import ProtocolException
from bridge import Bridge as BridgeProtocol
from bridge.protocol import Cmd

class Bridge(object):
    def __init__(self, slave_addr, timeout = 0.3, serial_timeout = 10,
                 *args, **kwargs):
        self.serial = serial.Serial(timeout = serial_timeout, *args, **kwargs)
        self.slave_addr = slave_addr
        self.bridge = BridgeProtocol(self.serial)
        self.timeout = timeout
        
    def query(self, request):
        now = time.clock()
        elapsed = now
        while elapsed - now < self.timeout:
            (send_status, send_remaining) = \
                                self.bridge.execute(Cmd.TWI_MASTER_SEND,
                                                    self.slave_addr,
                                                    request,
                                                    0)
#            pdb.set_trace()
            (recv_status, recv_remaining, reply) = \
                                self.bridge.execute(
                                    Cmd.TWI_MASTER_RECV,
                                    self.slave_addr,
                                    5, 1, 1)
#            pdb.set_trace()
            
            elapsed = time.clock()
            if send_remaining + recv_remaining != 0:
                print "send_remaining: {} status {:02x}".\
                    format(send_remaining, send_status)
                print "recv_remaining: {} status {:02x}".\
                    format(recv_remaining, recv_status)
                continue

            return reply

        raise ProtocolException("Failed to query")

    def close(self):
        self.serial.close()
        

