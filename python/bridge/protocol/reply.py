# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/bridge/protocol/reply.py -- basic stuff for constructing bridge replys
#

import pdb

from bridge.protocol.packet import Reply
from bridge.protocol import Cmd

class Ping(Reply):
    @classmethod
    def parse(cls, packet):
        cmd, content = super(Ping, cls).parse(packet)
        cls.assertTrue( cmd == Cmd.PING and len(content) == 4,
                        "Ping received wrong reply: {}".format(packet) )        
        return \
            (content[3] << 24)|\
            (content[2] << 16)|\
            (content[1] << 8)|\
            content[0]
    
Ping.register(Cmd.PING)

class TwiConfig(Reply):
    @classmethod
    def parse(cls, packet):
        cmd, content = super(TwiConfig, cls).parse(packet)
        cls.assertTrue( cmd == Cmd.TWI_CONFIG and len(content) == 0,
                        "TWI CONFIG received wrong reply: {}".format(packet) ) 
        return None
    
TwiConfig.register(Cmd.TWI_CONFIG)

class TwiMasterSend(Reply):
    @classmethod
    def parse(cls, packet):
        cmd, content = super(TwiMasterSend, cls).parse(packet)
        cls.assertTrue( cmd == Cmd.TWI_MASTER_SEND and len(content) == 2,
                        "TWI MASTER SEND received wrong reply: {}".\
                        format(packet) ) 
        return (content[0], content[1])
    
TwiMasterSend.register(Cmd.TWI_MASTER_SEND)

class TwiMasterRecv(Reply):
    @classmethod
    def parse(cls, packet):
        cmd, content = super(TwiMasterRecv, cls).parse(packet)
        cls.assertTrue( cmd == Cmd.TWI_MASTER_RECV and len(content) >= 2,
                        "TWI MASTER RECV received wrong reply: {}".\
                        format(packet) ) 
        return (content[0], content[1], content[2:])
    
TwiMasterRecv.register(Cmd.TWI_MASTER_RECV)
