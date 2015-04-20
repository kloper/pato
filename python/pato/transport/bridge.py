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

from util.protocol import ProtocolException
from bridge import Bridge as BridgeProtocol
from bridge.protocol import Cmd

class Bridge(object):
    """
    @brief Communication transport using Pato's UART-TWI bridge

    A simple transport that allows python code running on PC to talk
    with Pato via UART, while Pato is compiled with TWI (I2C) interface
    only.

    This requires python pyserial package to be installed.

    The main purpose of this transport is automatic E2D testing of various
    features.
    """

    def __init__(self, slave_addr, timeout=0.3, serial_timeout=10,
                 *args, **kwargs):
        """
        @brief Constructor

        @param[in] slave_addr TWI address of target Pato device
        @param[in] timeout timeout value in time.clock() units for
                           the query sequence to be completed.
        @param[in] serial_timeout timeout value for pyserial communication.
        @param[in] args extra arguments for pyserial
        @param[in] kwargs extra keyword arguments for pyserial
        """
        self.serial = serial.Serial(timeout=serial_timeout, *args, **kwargs)
        self.slave_addr = slave_addr
        self.bridge = BridgeProtocol(self.serial)
        self.timeout = timeout

    def query(self, request):
        """
        @brief Generic query (request/reply) method for UART-TWI bridge.

        Schedule TWI Write of request packet supplied as a parameter. The write
        is performed without stop condition. Then, TWI Read for a
        single reply packet is scheduled.

        If send and/or receive return unexpected result, the function will
        retry both send and receive pair until timeout specified in constructor
        is reached. If during the timeout period send and receive didn't
        succeeded, @ref ProtocolException is thrown.

        @param[in] request regular list of bytes representing packet to be sent
                           via the bridge.
        @returns Received reply packet
        @throws ProtocolException upon send or receive timeout
        """
        now = time.clock()
        elapsed = now
        while elapsed - now < self.timeout:
            (send_status, send_remaining) = \
                                self.bridge.execute(Cmd.TWI_MASTER_SEND,
                                                    self.slave_addr,
                                                    request,
                                                    0)
            (recv_status, recv_remaining, reply) = \
                                self.bridge.execute(
                                    Cmd.TWI_MASTER_RECV,
                                    self.slave_addr,
                                    5, 1, 1)

            elapsed = time.clock()
            if send_remaining + recv_remaining != 0:
                print("send_remaining: {} status {:02x}".\
                        format(send_remaining, send_status))
                print("recv_remaining: {} status {:02x}".\
                        format(recv_remaining, recv_status))
                continue

            return reply

        raise ProtocolException("Failed to query")

    def close(self):
        """
        @brief Close serial line to bridge
        """
        self.serial.close()
