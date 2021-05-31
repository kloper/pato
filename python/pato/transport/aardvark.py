# -*- python -*-
"""@file

@brief TotalPhase Aardwark transport for pato

Copyright (c) 2014-2021 Dimitry Kloper <kloper@users.sf.net>.
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
import sys
import os
from array import array

from pato.protocol import Reply

local_dir = os.path.abspath(os.path.dirname(__file__))
sys.path.append(os.path.abspath(os.path.join(local_dir,
    '..', '..', '..', '..', '..', 'totalphase', 'aardvark',
    'aardvark-api-windows-x86_64-v5.50', 'python')))

import serial
import time

from util.protocol import ProtocolException
import aardvark_py as AardvarkTransport

class Aardvark(object):
    def __init__(self, aardvark_port, slave_addr=0x41, bitrate=100):
        self.handle = AardvarkTransport.aa_open(aardvark_port)
        assert self.handle > 0, \
          f'Unable open aardvark on port {aardvark_port}: {self.handle}'
        AardvarkTransport.aa_configure(self.handle,  AardvarkTransport.AA_CONFIG_SPI_I2C)
        AardvarkTransport.aa_i2c_pullup(self.handle, AardvarkTransport.AA_I2C_PULLUP_NONE)
        self.bitrate = AardvarkTransport.aa_i2c_bitrate(self.handle, bitrate)
        self.slave_addr = slave_addr

    def query(self, request):
        AardvarkTransport.aa_i2c_write(
            self.handle, self.slave_addr,
            AardvarkTransport.AA_I2C_NO_FLAGS,
            array('B', request))
        time.sleep(0.05)
        while True:
            (count, reply) = AardvarkTransport.aa_i2c_read(
                self.handle, self.slave_addr,
                AardvarkTransport.AA_I2C_NO_FLAGS,
                len(request))
            if count == len(request) and reply[0] in [Reply.OK, Reply.ERROR]:
                break
            time.sleep(0.05)

        return reply.tobytes()

    def close(self):
        AardvarkTransport.aa_close(self.handle)
