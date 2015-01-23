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

class TwiConfig(Request):
    @classmethod
    def compile(cls, prescaler, divider):
        cls.assertTrue( prescaler in range(4),
                        "prescaler value must be in range [0..3]" )
        cls.assertTrue( divider >= 0 and divider < 256,
                        "divider value must be in range [0..255]" )

        val = (prescaler << 8) | divider
        return super(TwiConfig, cls).compile([ val & 0xff, (val >> 8) & 0xff])

TwiConfig.register(Cmd.TWI_CONFIG)

class TwiMasterSend(Request):
    @classmethod
    def compile(cls, addr, data, stop):
        cls.assertTrue( addr >= 0 and addr < 128,
                        "slave address value must be in range [0..127]" )
        cls.assertTrue( len(data) < 250,
                        "data length can't be greater than 250 bytes" )
        return super(TwiMasterSend, cls).compile(
            [ stop & 0xff, addr & 0xff] + data
        )

TwiMasterSend.register(Cmd.TWI_MASTER_SEND)

class TwiMasterRecv(Request):
    @classmethod
    def compile(cls, addr, size, stop = 1, last_nack = 1):
        cls.assertTrue( addr >= 0 and addr < 128,
                        "slave address value must be in range [0..127]" )
        cls.assertTrue( size > 0 and size < 250,
                        "size must be in range [0..249]" )
        return super(TwiMasterRecv, cls).compile(
            [ stop & 0xff, last_nack & 0xff, addr & 0xff, size & 0xff] 
        )

TwiMasterRecv.register(Cmd.TWI_MASTER_RECV)
