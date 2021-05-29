# -*- python -*-
"""@file

@brief pyserial transport for pato

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

import serial

from util.protocol import ProtocolException

class Uart(object):
    """
    @brief Communication transport using any UART TTL cable (FTDI)

    A simple transport that allows python code running on PC to talk
    with Pato via UART (using any UART cable or dongle e.g. FTDI),
    while Pato is compiled with UART interface.

    This requires python pyserial package to be installed.
    """

    def __init__(self, *args, **kwargs):
        """
        @brief Constructor

        @param[in] args arguments for pyserial
        @param[in] kwargs keyword arguments for pyserial
        """

        self.serial = serial.Serial(*args, **kwargs)

    def query(self, request):
        """
        @brief Generic query (request/reply) method via pyserial interface.

        Send request packet to Pato via serial interface and wait for reply
        packet.

        If send and/or receive return unexpected result,
        @ref ProtocolException is thrown.

        @param[in] request regular list of bytes representing packet to be sent
                           via the bridge.
        @returns Received reply packet
        @throws ProtocolException upon send or receive error
        """
        bytes_written = self.serial.write(bytes(request))

        if bytes_written != len(request):
            raise ProtocolException("Failed to send request")

        reply_size = 5
        reply = self.serial.read(reply_size)
        if len(reply) != reply_size:
            raise ProtocolException("Failed to receive reply")

        reply = [ord(c) for c in reply]
        return reply

    def close(self):
        """
        @brief Close serial line to bridge
        """
        self.serial.close()
