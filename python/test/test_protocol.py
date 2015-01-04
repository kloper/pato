# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/test/test_protocol.py -- test for protocol stuff
#

import os
import sys
import pdb
import unittest
import traceback
import datetime
import time
import serial

import logging
import logging.config

localdir = os.path.dirname( os.path.realpath(__file__) );
sys.path.append( os.path.join( localdir, '..') );

from pato import Pato
from pato.protocol import Cmd, Direct

logging.config.dictConfig( 
    { 'version': 1,              
      'disable_existing_loggers': True,
      'formatters': {
          'standard': {
              'format': '%(asctime)s:%(levelname)s:'
              '%(filename)s:%(lineno)d: %(message)s'
          },
      },
      'handlers': {
          'default': {
              'level': 'DEBUG',    
              'class': "logging.StreamHandler",
              'stream': sys.stdout,
              'formatter': 'standard'
          },  
          'file': {
              'level': 'DEBUG',    
              'class': "logging.FileHandler",
              'filename': "{}-{}.log".\
              format( __file__, datetime.datetime.now().\
                      strftime("%d%m%Y-%H%M%S")),              
              'formatter': 'standard'
          },  
      },
      'loggers': {
          'default': {                  
              'handlers': ['default', 'file'], 
              'level': 'DEBUG',  
              'propagate': False
          },
    },
  }
)

def pairs(str):
    strlen = len(str) + len(str)%2
    str += "\0"
    for i in xrange(0, strlen, 2):
        yield str[i:i+2]

def int2str(val, size = 2):
    res = []
    for _ in xrange(size):
        res.append( val & 0xff )
        val >>= 8
    return "".join(chr(c) for c in reversed(res))


class Test(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.logger = logging.getLogger('default')
        cls.transport = serial.Serial(port = 'COM83',
                                      baudrate = 9600,
                                      timeout = 10)

    @classmethod
    def tearDownClass(cls):
        cls.transport.close()                
    
    def test_ping(self):
        pato = Pato(self.transport)
        rc = pato.execute(Cmd.PING, 0)
        self.assertTrue(rc == 1)

    def test_write(self):
        pato = Pato(self.transport)
        for c in "Hello World":
            rc = pato.execute(Cmd.DIRECT, Direct.WRITE, ord(c))

    def test_read(self):
        pato = Pato(self.transport)

        pato.execute(Cmd.RESET, 0)
        pato.execute(Cmd.DIRECT, Direct.DCTRL, True, True, True)
        pato.execute(Cmd.DIRECT, Direct.EMS, True, False)
        
        str = "Hello World"
        for c in str:
            rc = pato.execute(Cmd.DIRECT, Direct.WRITE, ord(c))
            
        pato.execute(Cmd.DIRECT, Direct.DDADDR, 0)
        pato.execute(Cmd.DIRECT, Direct.READ)

        res = ""
        for _ in xrange(len(str)):
            (addr, char) = pato.execute(Cmd.DIRECT, Direct.READ)
            res += chr(char)

        self.assertTrue( res == str )

    def reset_pato(self, pato):
        pato.execute(Cmd.RESET, 0)
        pato.execute(Cmd.DIRECT, Direct.DCTRL, True, True, True)
        pato.execute(Cmd.DIRECT, Direct.EMS, True, False)
        
    def test_print(self):
        pato = Pato(self.transport)

        self.reset_pato(pato)

        pato.execute(Cmd.PRINT_SETADDR, 0)
        str = "Hello World!!! 1 2 3 4 5 \0"
        for pair in pairs(str):
            pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))
        pato.execute(Cmd.PRINT_COMMIT)

    def test_print_ex(self):
        pato = Pato(self.transport)

        self.reset_pato(pato)

        pato.execute(Cmd.PRINT_SETADDR, 0)
        prefix = "Counter %08x\0"
        str = prefix+int2str(0)
        for pair in pairs(str):
            pato.execute(Cmd.PRINT_PUT, ord(pair[0]), ord(pair[1]))

        for i in xrange(100):
            pato.execute(Cmd.PRINT_SETADDR, len(prefix))
            pato.execute(Cmd.PRINT_PUT, i & 0xff, (i >> 8) & 0xff) 
            pato.execute(Cmd.PRINT_COMMIT)
            time.sleep(1)
        
