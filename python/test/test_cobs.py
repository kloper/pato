# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/test/test_cobs.py -- test util.cobs
#

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
