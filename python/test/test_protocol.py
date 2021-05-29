# -*- python -*-
"""@file

@brief test for protocol stuff

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
import pdb
import unittest
import time
import math

localdir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(localdir, '..'))

from pato import Pato
from pato.protocol import Cmd, Direct
from pato.transport.uart import Uart
from pato.transport.bridge import Bridge, ProtocolException
from pato.transport.aardvark import Aardvark

from util import float2str, int2str, pairs, range as xrange

from bridge.protocol import Cmd as BridgeCmd

from test import logger

class NullTest(unittest.TestCase):
    transport = None
    logger = None

    def test_ping(self):
        pato = Pato(self.transport)
        for i in xrange(100):
            breakpoint()
            rc = pato.execute(Cmd.PING, i & 0xff)
            self.logger.info("pong: %s", rc)
            self.assertTrue(rc == (i + 1) & 0xff)

    def test_write(self):
        pato = Pato(self.transport)
        for txt in ["Hello World"] * 10:
            rc = pato.execute(Cmd.DIRECT, Direct.CLR)
            for c in txt:
                rc = pato.execute(Cmd.DIRECT, Direct.WRITE, ord(c))
                # pdb.set_trace()

    def test_read(self):
        pato = Pato(self.transport)

        self.reset_pato(pato)

        txt = "Hello World"
        for c in txt:
            rc = pato.execute(Cmd.DIRECT, Direct.WRITE, ord(c))

        pato.execute(Cmd.DIRECT, Direct.DDADDR, 0)

        res = ""
        for _ in xrange(len(txt)):
            (addr, char) = pato.execute(Cmd.DIRECT, Direct.READ)
            res += chr(char)

        pdb.set_trace()
        self.assertTrue(res == txt)

    def reset_pato(self, pato):
        pato.execute(Cmd.RESET, 0)
        pato.execute(Cmd.DIRECT, Direct.DCTRL, True, True, True)
        pato.execute(Cmd.DIRECT, Direct.EMS, True, False)

    def test_print(self):
        breakpoint()
        pato = Pato(self.transport)

        self.reset_pato(pato)

        pato.execute(Cmd.PRINT_SETADDR, 0)
        txt = "Hello World!!! 1 2 3 4 5 \0"
        for pair in pairs(txt):
            pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))
        pato.execute(Cmd.PRINT_COMMIT, 0)

    def test_print_ptr(self):
        pato = Pato(self.transport)

        self.reset_pato(pato)

        pato.execute(Cmd.PRINT_SETADDR, 0)
        fmt = "\fName: %s\n\t(%d)\0"
        for pair in pairs(fmt):
            pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))
        name_offset = pato.execute(Cmd.PRINT_GETADDR)

        for name in ["pato \0", "pocoyo \0", "sleepy bird\0"]:
            pato.execute(Cmd.PRINT_SETADDR, name_offset)
            for pair in pairs(name):
                pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))

            params_offset = pato.execute(Cmd.PRINT_GETADDR)
            pato.execute(Cmd.PRINT_PUT_PTR, name_offset)

            for pair in pairs(int2str(len(name))):
                pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))

            pato.execute(Cmd.PRINT_COMMIT, params_offset)
            time.sleep(1)

    def test_print_ex(self):
        pato = Pato(self.transport)

        self.reset_pato(pato)

        pato.execute(Cmd.PRINT_SETADDR, 0)
        prefix = "\fCounter A %08x ****\n" \
                 "Counter B %08x ----\n" \
                 "Counter C %d $$$$\n" \
                 "Total: %d\0"
        txt = prefix+int2str(0)+int2str(0)+int2str(0)+int2str(0)
        for pair in pairs(txt):
            pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))

        for i in xrange(30):
            for j in xrange(20):
                for k in xrange(10):
                    pato.execute(Cmd.PRINT_SETADDR, len(prefix))
                    pato.execute(Cmd.PRINT_PUT, i & 0xff, (i >> 8) & 0xff)
                    pato.execute(Cmd.PRINT_PUT, j & 0xff, (j >> 8) & 0xff)
                    pato.execute(Cmd.PRINT_PUT, k & 0xff, (k >> 8) & 0xff)
                    total = i + j + k
                    pato.execute(Cmd.PRINT_PUT,
                                 total & 0xff,
                                 (total >> 8) & 0xff)
                    pato.execute(Cmd.PRINT_COMMIT, 0)

    def test_print_float(self):
        pato = Pato(self.transport)

        self.reset_pato(pato)

        pato.execute(Cmd.PRINT_SETADDR, 0)
        def send(prefix, t, sin, cos, tan):
            s = prefix + float2str(t) + float2str(sin) + \
                float2str(cos) + float2str(tan)
            for pair in pairs(s):
                pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))

        prefix = "\ft = %f\n" \
                 "sin(t) = %f\n" \
                 "cos(t) = %f\n" \
                 "tan(t) = %f\0"

        send(prefix, 0, 0, 0, 0)

        for i in xrange(10000):
            t = i * 0.001
            pato.execute(Cmd.PRINT_SETADDR, len(prefix))
            send("", t, math.sin(t), math.cos(t), math.tan(t))
            pato.execute(Cmd.PRINT_COMMIT, 0)

    def test_print_overrun(self):
        pato = Pato(self.transport)

        self.reset_pato(pato)

        pato.execute(Cmd.PRINT_SETADDR, 0)
        line1 = "\fLine #1\n" \
                "Line #2\n" \
                "Line #3\n" \
                "Line #4\n" \
                "Line #5\n\0"
        line2 = "Line #6\n" \
                "Line #7\n" \
                "Line #8\n\0"
        pato.execute(Cmd.PRINT_SETADDR, 0)
        for pair in pairs(line1):
            pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))
        pato.execute(Cmd.PRINT_COMMIT, 0)
        time.sleep(2)
        pato.execute(Cmd.PRINT_SETADDR, 0)
        for pair in pairs(line2):
            pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))
        pato.execute(Cmd.PRINT_COMMIT, 0)

class UartTransport(NullTest):
    @classmethod
    def setUpClass(cls):
        cls.logger = logger
        cls.transport = Uart(port='COM5', baudrate=9600, timeout=10)

    @classmethod
    def tearDownClass(cls):
        cls.transport.close()

class AardvarkTransport(NullTest):
    @classmethod
    def setUpClass(cls):
        cls.logger = logger
        cls.transport = Aardvark(aardvark_port=0, slave_addr=0x41)

    @classmethod
    def tearDownClass(cls):
        cls.transport.close()
        
class BridgeTransport(NullTest):
    @classmethod
    def setUpClass(cls):
        cls.logger = logger
        cls.transport = Bridge(slave_addr=0x41,
                               port='COM7',
                               baudrate=57600,
                               timeout=10)
        cls.query_method = cls.transport.query

    @classmethod
    def tearDownClass(cls):
        cls.transport.close()

    def tearDown(self):
        self.transport.query = self.query_method

    def out_of_sync_query(self, request):
        (status, remaining) = self.transport.bridge.execute(
            BridgeCmd.TWI_MASTER_SEND,
            self.transport.slave_addr,
            request,
            0)

        assert remaining == 0, "Failed to send req #1"
        (status, remaining) = self.transport.bridge.execute(
            BridgeCmd.TWI_MASTER_SEND,
            self.transport.slave_addr,
            request,
            0)

        assert remaining != 0, "Req #2 succeeded while shouldn't"

        (status, remaining) = self.transport.bridge.execute(
            BridgeCmd.TWI_MASTER_SEND,
            self.transport.slave_addr,
            request,
            0)

        assert remaining != 0, "Req #3 succeeded while shouldn't"

        time.sleep(0.3)
        (status, remaining, reply) = self.transport.bridge.execute(
            BridgeCmd.TWI_MASTER_RECV,
            self.transport.slave_addr,
            5, 1, 1)

        assert remaining == 0, "Failed to receive reply #1"

        (status, remaining, reply2) = self.transport.bridge.execute(
            BridgeCmd.TWI_MASTER_RECV,
            self.transport.slave_addr,
            5, 1, 1)

        assert remaining == 0, "Failed to receive reply #2"
        assert sum(reply2) == 0, "Reply #2 has non-zero bytes"

        return reply

    def short_packet_query(self, request):
        if not hasattr(self, 'count'):
            setattr(self, 'count', 0)

        self.count += 1

        (status, remaining) = self.transport.bridge.execute(
            BridgeCmd.TWI_MASTER_SEND,
            self.transport.slave_addr,
            request[0:(self.count % 5)] + [5]*(self.count%10),
            0)

        time.sleep(0.3)
        (status, remaining, reply) = self.transport.bridge.execute(
            BridgeCmd.TWI_MASTER_RECV,
            self.transport.slave_addr,
            5, 1, 1)

        return reply

    def test_unsync_ping(self):
        self.transport.query = self.out_of_sync_query
        return self.test_ping()

    def test_short_ping(self):
        self.transport.query = self.short_packet_query

        pato = Pato(self.transport)
        for i in xrange(100):
            self.assertRaises(ProtocolException,
                              pato.execute, Cmd.PING, i & 0xff)
