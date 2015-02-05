# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/transport/bridge.py -- i2c-serial transport for pato
#

from __future__ import absolute_import

import serial
import time
import pdb

from util.protocol import ProtocolException
from bridge import Bridge as BridgeProtocol
from bridge.protocol import Cmd

class Bridge(object):
    def __init__(self, slave_addr, timeout = 0.2, serial_timeout = 10,
                 *args, **kwargs):
        self.serial = serial.Serial(timeout = serial_timeout, *args, **kwargs)
        self.slave_addr = slave_addr
        self.bridge = BridgeProtocol(self.serial)
        self.timeout = timeout
        
    def query(self, request):
        now = time.clock()
        elapsed = now
        while elapsed - now < self.timeout:
            (status, send_remaining) = self.bridge.execute(Cmd.TWI_MASTER_SEND,
                                                           self.slave_addr,
                                                           request,
                                                           0)
            
            (status, recv_remaining, reply) = self.bridge.execute(
                Cmd.TWI_MASTER_RECV,
                self.slave_addr,
                5, 1, 1)
            
            elapsed = time.clock()
            if send_remaining + recv_remaining != 0:
                continue

            return reply

        raise ProtocolException("Failed to query")

    def close(self):
        self.serial.close()
        

