# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/bridge/protocol/request.py -- basic stuff for constructing
#                                      bridge requests
#

import pdb

from bridge.protocol.packet import Request
from bridge.protocol import Cmd

class Ping(Request):
    @classmethod
    def compile(cls, value):
        cls.assertTrue( value >= 0 and value <= 0xffffffff,
                         "value must be in range [0..0xffffffff]" )
        return super(Ping, cls).compile([ value & 0xff,
                                         (value >> 8) & 0xff,
                                         (value >> 16) & 0xff,
                                         (value >> 24) & 0xff])

Ping.register(Cmd.PING)
