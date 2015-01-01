# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/protocol/packet.py -- basic stuff for constructing Pato packets
#

import pdb

from collections import defaultdict

from pato.protocol.crc import crc8
from pato.protocol import Reply as ReplyVal, Error, ProtocolException

def tree(): return defaultdict(tree)

class PacketBase(object):
    @classmethod
    def register(cls, *args):
        assert len(args) > 0, "register expects one or more arguments"
        reg = cls.Registry
        for cmd in args[0:-1]:
            reg = reg[cmd]
        assert args[-1] not in reg, "Command with ID {} is already registered".\
            format(args)
        reg[args[-1]] = cls
        cls.cmd = args[0]

    @classmethod
    def find(cls, *args):
        assert len(args) > 0, "find expects one or more arguments"
        reg = cls.Registry
        for cmd in args:
            if cmd in reg:
                reg = reg[cmd]
            else:
                return None
        return reg
        
    @staticmethod
    def assertTrue(expr, *args, **kwargs):
        if not expr:
            raise ProtocolException(*args, **kwargs)

    @staticmethod
    def assertFalse(expr, *args, **kwargs):
        if expr:
            raise ProtocolException(*args, **kwargs)

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
