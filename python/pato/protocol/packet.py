# -*- python -*-
"""@file

@brief Basic stuff for constructing Pato packets

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

from util.packet import PacketBase, tree
from util.crc import crc8

from pato.protocol import Reply as ReplyVal

class Request(PacketBase):
    """
    @brief Base class for all request packets.

    In Java terms this is a static class. It introduces a single static
    method Request.compile() that is able to make a valid ready to transfer
    packet with arbitrary payload.

    All request packets share the same registry. The registry is used by
    PacketBase apparatus to find packet handles by id.

    Id of request packet is id of associated command (see
    @ref pato.protocol.Cmd). In case of DIRECT command, the id also includes
    the sub-command (see @ref pato.protocol.Direct).
    """
    Registry = tree()

    @classmethod
    def compile(cls, payload=None):
        """
        @brief Make request packet with arbitrary payload.

        The command value for the packet is taken from Request.cmd that
        is set by Request.register() command.
        @param[in] payload sequence containing two bytes of payload
        @returns list containing byte sequence representing the packet.
        """
        cls.assert_true(len(payload) <= 2, "Payload must be 2 bytes")

        res = [cls.cmd, payload[0], payload[1]]
        res += [crc8(res), 0]
        return res

class Reply(PacketBase):
    """
    @brief Base class for all reply packets.

    In Java terms this is a static class. It introduces a single static
    method Reply.parse() that is able to verify reply packet and extract
    its payload.

    All reply packets share the same registry. The registry is used by
    PacketBase apparatus to find packet handles by id.

    Id of request packet is id of associated command (see
    @ref pato.protocol.Cmd). In case of DIRECT command, the id also includes
    the sub-command (see @ref pato.protocol.Direct).
    """
    Registry = tree()

    @classmethod
    def parse(cls, packet):
        """
        @brief Take raw byte sequence verify it and return payload.

        The packet byte sequence is verified for CRC, separator value
        (last zero). If any verification fails or when this is a valid error
        packet this method will throw ProtocolException.

        @param[in] packet sequence containing reply packet
        @returns list containing byte sequence of packet payload
        @throws ProtocolException
        """

        cls.assert_true(len(packet) == 5, "Packet length must be 5")

        cls.assert_true(packet[4] == 0,
                        msg="Bad separator",
                        packet=packet)

        cls.assert_true(packet[3] == crc8(packet[0:3]),
                        msg="Bad CRC",
                        packet=packet)

        cls.assert_true(packet[0] != ReplyVal.ERROR,
                        msg="Error response",
                        error_code=packet[1],
                        cmd=packet[2] )

        return packet[1:3]
