# -*- python -*-
"""@file

@brief Basic stuff for constructing bridge replies

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

from bridge.protocol.packet import Reply
from bridge.protocol import Cmd

class Ping(Reply):
    @classmethod
    def parse(cls, packet):
        cmd, content = super(Ping, cls).parse(packet)
        cls.assertTrue( cmd == Cmd.PING and len(content) == 4,
                        "Ping received wrong reply: {}".format(packet) )        
        return \
            (content[3] << 24)|\
            (content[2] << 16)|\
            (content[1] << 8)|\
            content[0]
    
Ping.register(Cmd.PING)

class TwiConfig(Reply):
    @classmethod
    def parse(cls, packet):
        cmd, content = super(TwiConfig, cls).parse(packet)
        cls.assertTrue( cmd == Cmd.TWI_CONFIG and len(content) == 0,
                        "TWI CONFIG received wrong reply: {}".format(packet) ) 
        return None
    
TwiConfig.register(Cmd.TWI_CONFIG)

class TwiMasterSend(Reply):
    @classmethod
    def parse(cls, packet):
        cmd, content = super(TwiMasterSend, cls).parse(packet)
        cls.assertTrue( cmd == Cmd.TWI_MASTER_SEND and len(content) == 2,
                        "TWI MASTER SEND received wrong reply: {}".\
                        format(packet) ) 
        return (content[0], content[1])
    
TwiMasterSend.register(Cmd.TWI_MASTER_SEND)

class TwiMasterRecv(Reply):
    @classmethod
    def parse(cls, packet):
        cmd, content = super(TwiMasterRecv, cls).parse(packet)
        cls.assertTrue( cmd == Cmd.TWI_MASTER_RECV and len(content) >= 2,
                        "TWI MASTER RECV received wrong reply: {}".\
                        format(packet) ) 
        return (content[0], content[1], content[2:])
    
TwiMasterRecv.register(Cmd.TWI_MASTER_RECV)
