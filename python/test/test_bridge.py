# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/test/test_bridge.py -- test bridge protocol
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

from bridge import Bridge
from bridge.protocol import Cmd, Error

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
                                      baudrate = 57600,
                                      timeout = 10)

    @classmethod
    def tearDownClass(cls):
        cls.transport.close()                
    
    def test_ping(self):
        bridge = Bridge(self.transport)
        for i in xrange(10000):
            rc = bridge.execute(Cmd.PING, i)
            self.logger.info("pong: {}".format(rc))
            self.assertTrue(rc == i+1)
