# -*- python -*-
"""@file

@brief test bridge protocol

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

import os
import sys
import unittest
import serial

localdir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(localdir, '..'))

from bridge import Bridge
from bridge.protocol import Cmd
from util.protocol import ProtocolException
from util import range as xrange

from test import logger

class Test(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.logger = logger
        cls.transport = serial.Serial(port='COM102',
                                      baudrate=57600,
                                      timeout=1)

    @classmethod
    def tearDownClass(cls):
        cls.transport.close()

    def test_ping(self):
        bridge = Bridge(self.transport)
        for i in xrange(100):
            try:
                rc = bridge.execute(Cmd.PING, i)
                self.logger.info("pong: %s", rc)
                self.assertTrue(rc == i+1)
            except ProtocolException:
                self.logger.warning("Protocol Exception", exc_info=True)

    def test_master_send(self):
        bridge = Bridge(self.transport)
        for _ in xrange(1):
            rc = bridge.execute(Cmd.TWI_MASTER_SEND, 0x41,
                                [1, 2, 5, 0xa, 0xe], 1)
            self.logger.info("sent: %s", rc)

    def test_master_recv(self):
        bridge = Bridge(self.transport)
        for _ in xrange(100):
            rc = bridge.execute(Cmd.TWI_MASTER_RECV, 0x41, 9, 1, 1)
            self.logger.info("received: %s", rc)

    def test_at24c02_read(self):
        bridge = Bridge(self.transport)
        for addr in xrange(10):
            rc = bridge.execute(Cmd.TWI_MASTER_SEND, 0x50, [addr], 0)
            self.logger.info("sent: %s", rc)
            self.assertTrue(rc[1] == 0, "Send failed")
            size = 10
            rc = bridge.execute(Cmd.TWI_MASTER_RECV, 0x50, size, 1, 1)
            self.logger.info("received: %s", rc)
            self.assertTrue(rc[1] == 0 and len(rc[2]) == size, "Recv failed")

    def test_at24c02_write(self):
        bridge = Bridge(self.transport)
        for addr in xrange(1):
            rc = bridge.execute(Cmd.TWI_MASTER_SEND, 0x50,
                                [addr, 0xeb, 0xfc], 1)
            self.logger.info("sent: %s", rc)

    def test_pato_ping(self):
        bridge = Bridge(self.transport)

        rc = bridge.execute(Cmd.TWI_MASTER_SEND, 0x41,
                            [0, 0, 0, 0, 0], 0)
        self.logger.info("sent: %s", rc)

        rc = bridge.execute(Cmd.TWI_MASTER_RECV, 0x41, 5, 1, 1)
        self.logger.info("received: %s", rc)
        #self.assertTrue( rc[1] == 0 and len(rc[2]) == size, "Recv failed")
