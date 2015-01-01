# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/test/test_packet.py -- driver for sending raw packets to pato
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

import pato.protocol as protocol
from pato.protocol.crc import crc8

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

    def test_send_single_ping(self):
        packet = [ protocol.Cmd.PING, 0, 0 ]
        packet += [ crc8(packet), 0 ]
        packet = "".join( [chr(c) for c in packet] )

        pdb.set_trace()
        self.transport.write(packet)
