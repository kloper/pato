# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/protocol/packet.py -- basic stuff for constructing Pato packets
#

import pdb

from collections import defaultdict

from util.packet import PacketBase, tree
from util.protocol import ProtocolException
from util.crc import crc8

from pato.protocol import Reply as ReplyVal, Error

class Request(PacketBase):
    Registry = tree()
    
    @classmethod
    def compile(cls, payload = []):
        cls.assertTrue( len(payload) <= 2, "Payload must be 2 bytes" )

        res = [cls.cmd, payload[0], payload[1]]
        res += [crc8(res), 0]
        return res

class Reply(PacketBase):
    Registry = tree()
    
    @classmethod
    def parse(cls, packet):
        cls.assertTrue( len(packet) == 5, "Packet length must be 5" )

        cls.assertTrue( packet[4] == 0,
                         msg = "Bad separator",
                         packet = packet)

        cls.assertTrue( packet[3] == crc8(packet[0:3]),
                         msg = "Bad CRC",
                         packet = packet)

        cls.assertTrue( packet[0] != ReplyVal.ERROR,
                         msg = "Error responce",
                         error_code = packet[1],
                         cmd = packet[2] )

        return packet[1:3]
