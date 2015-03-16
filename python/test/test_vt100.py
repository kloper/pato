# -*- python -*-
"""@file

@brief test v100 stuff

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
import traceback
import time
import serial
import math
import struct

localdir = os.path.dirname( os.path.realpath(__file__) )
sys.path.append( os.path.join( localdir, '..') )

from pato import Pato
from pato.protocol import Cmd, Direct
from pato.transport.uart import Uart
from pato.transport.bridge import Bridge, ProtocolException

from bridge.protocol import Cmd as BridgeCmd    

from util import pairs, int2str
from test import logger

class NullTest(unittest.TestCase):
    def reset_pato(self, pato):
        pato.execute(Cmd.RESET, 0)
        pato.execute(Cmd.DIRECT, Direct.DCTRL, True, True, True)
        pato.execute(Cmd.DIRECT, Direct.EMS, True, False)
    
    def test_tab(self):
        pato = Pato(self.transport)

        self.reset_pato(pato)

        pato.execute(Cmd.PRINT_SETADDR, 0)
        prefix = "\f"\
                 "\t%02x\t%0x\t%02x\t%02x\n"\
                 "\0"
        for pair in pairs(prefix):
            pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))

        for i in xrange(100):
            pato.execute(Cmd.PRINT_SETADDR, len(prefix))
            for j in xrange(1,5):
                pato.execute(Cmd.PRINT_PUT,  (i+j) & 0xff, ((i+j) >> 8) & 0xff)
            pato.execute(Cmd.PRINT_COMMIT)
                    

class UartTransport(NullTest):
    @classmethod
    def setUpClass(cls):
        cls.logger = logger
        cls.transport = Uart(port = 'COM83',
                             baudrate = 9600,
                             timeout = 10)
        
    @classmethod
    def tearDownClass(cls):
        cls.transport.close()                

class BridgeTransport(NullTest):
    @classmethod
    def setUpClass(cls):
        cls.logger = logger
        cls.transport = Bridge(slave_addr = 0x41,
                               port = 'COM97',
                               baudrate = 57600,
                               timeout = 10)
        cls.query_method = cls.transport.query

    @classmethod
    def tearDownClass(cls):
        cls.transport.close()                

