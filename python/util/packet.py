# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/util/packet.py -- basic stuff for constructing protocols
#

import pdb

from collections import defaultdict
from util.protocol import ProtocolException

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
