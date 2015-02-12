# -*- python -*-
"""@file

@brief Basic stuff for constructing bridge packets

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

import util.cobs as cobs

from util.packet import PacketBase, tree
from util.protocol import ProtocolException
from util.crc import crc16

class Request(PacketBase):
    Registry = tree()
    
    @classmethod
    def compile(cls, payload = []):
        res = [cls.cmd]+payload
        size = len(payload)+1
        crc = crc16(res)
        res = cobs.encode(
            [size & 0xff, crc & 0xff, (crc >> 8) & 0xff] + res
        ) + [0]
        return res

class Reply(PacketBase):
    Registry = tree()
    
    @classmethod
    def parse(cls, packet):
        cls.assertTrue( packet[-1] == 0 and min(packet[:-1]) > 0,
                        "Packet is not COBS encoded: {}".format(packet) )

        packet = cobs.decode(packet)
        size = packet[0]
        cls.assertTrue( size == len(packet)-3,
                        msg = "Bad Size",
                        packet = packet)

        
        crc = (packet[2] << 8) | packet[1]
        cls.assertTrue( crc == crc16(packet[3:]),
                        msg = "Bad CRC",
                        packet = packet)

        cls.assertTrue( packet[3] > 0 ,
                         msg = "Error responce",
                         error_code = packet[5],
                         cmd = packet[4] )

        return (packet[3], packet[4:])
