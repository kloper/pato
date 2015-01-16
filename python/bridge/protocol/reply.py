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
