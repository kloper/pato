# -*- python -*-
"""@file

@brief test util.cobs

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

localdir = os.path.dirname( os.path.realpath(__file__) );
sys.path.append( os.path.join( localdir, '..') );

from util.cobs import encode, decode, seq_join, seq_split

class CobsTest(unittest.TestCase):
    def test_encode_decode(self):
        content = seq_join([[1, 2, 3, 4, 5]]*10, 0)
        enc = encode(content)
        self.assertTrue(min(enc) > 0, "invalid enc")
        dec = decode(enc)
        self.assertTrue(cmp(content, dec) == 0, "invalid dec")

    def test_single_chunk(self):
        content = [1]*254
        enc = encode(content)
        self.assertTrue(min(enc) > 0, "invalid enc")
        self.assertTrue(len(enc) == 255, "invalid enc len")
        self.assertTrue(enc[0] == 255, "invalid enc prefix")
        dec = decode(enc)
        self.assertTrue(cmp(content, dec) == 0, "invalid dec")

    def test_small_chunk(self):
        content = [0]
        enc = encode(content)
        self.assertTrue(min(enc) > 0, "invalid enc")
        self.assertTrue(len(enc) == 2, "invalid enc len")
        dec = decode(enc)
        self.assertTrue(cmp(content,dec) == 0, "invalid dec")
        
    def test_chunk_length_bounds(self):
        content = []
        self.assertRaises(AssertionError, encode, content)
        self.assertRaises(AssertionError, decode, content)
        content = [1]*255
        self.assertRaises(AssertionError, encode, content)
        dec = decode(content)
        self.assertTrue( len(dec) != 0, "invalid dec") 
