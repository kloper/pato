# -*- python -*-
"""@file

@brief Basic stuff for constructing bridge requests

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

import pdb

from bridge.protocol.packet import Request
from bridge.protocol import Cmd

class Ping(Request):
    @classmethod
    def compile(cls, value):
        cls.assert_true( value >= 0 and value <= 0xffffffff,
                         "value must be in range [0..0xffffffff]" )
        return super(Ping, cls).compile([ value & 0xff,
                                         (value >> 8) & 0xff,
                                         (value >> 16) & 0xff,
                                         (value >> 24) & 0xff])

Ping.register(Cmd.PING)

class TwiConfig(Request):
    @classmethod
    def compile(cls, prescaler, divider):
        cls.assert_true( prescaler in range(4),
                        "prescaler value must be in range [0..3]" )
        cls.assert_true( divider >= 0 and divider < 256,
                        "divider value must be in range [0..255]" )

        val = (prescaler << 8) | divider
        return super(TwiConfig, cls).compile([ val & 0xff, (val >> 8) & 0xff])

TwiConfig.register(Cmd.TWI_CONFIG)

class TwiMasterSend(Request):
    @classmethod
    def compile(cls, addr, data, stop):
        cls.assert_true( addr >= 0 and addr < 128,
                        "slave address value must be in range [0..127]" )
        cls.assert_true( len(data) < 250,
                        "data length can't be greater than 250 bytes" )
        return super(TwiMasterSend, cls).compile(
            [ stop & 0xff, addr & 0xff] + data
        )

TwiMasterSend.register(Cmd.TWI_MASTER_SEND)

class TwiMasterRecv(Request):
    @classmethod
    def compile(cls, addr, size, stop = 1, last_nack = 1):
        cls.assert_true( addr >= 0 and addr < 128,
                        "slave address value must be in range [0..127]" )
        cls.assert_true( size > 0 and size < 250,
                        "size must be in range [0..249]" )
        return super(TwiMasterRecv, cls).compile(
            [ stop & 0xff, last_nack & 0xff, addr & 0xff, size & 0xff]
        )

TwiMasterRecv.register(Cmd.TWI_MASTER_RECV)
