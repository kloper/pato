# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/protocol/reply.py -- basic stuff for constructing Pato replys
#

from pato.protocol.packet import Reply
from pato.protocol import Cmd, Direct, Reply as ReplyVal, Error

class Ping(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(Ping, cls).parse(packet)
        cls.assertTrue( arg0 == Cmd.PING, "Ping received wrong reply: {}".\
                         format(packet) )
        return arg1
    
Ping.register(Cmd.PING)

class ClearScreen(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(ClearScreen, cls).parse(packet)
        return arg0
    
ClearScreen.register(Cmd.DIRECT, Direct.CLR)

class Home(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(Home, cls).parse(packet)
        return arg0
    
Home.register(Cmd.DIRECT, Direct.HOME)

class EntryModeSet(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(EntryModeSet, cls).parse(packet)
        return arg0
    
EntryModeSet.register(Cmd.DIRECT, Direct.EMS)

class DisplayControl(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(DisplayControl, cls).parse(packet)
        return arg0
    
DisplayControl.register(Cmd.DIRECT, Direct.DCTRL)

class Shift(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(Shift, cls).parse(packet)
        return arg0
    
Shift.register(Cmd.DIRECT, Direct.SHIFT)

class FunctionSet(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(FunctionSet, cls).parse(packet)
        return arg0
    
FunctionSet.register(Cmd.DIRECT, Direct.FUNC)

class SetCGRAMAddr(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(SetCGRAMAddr, cls).parse(packet)
        return arg0
    
SetCGRAMAddr.register(Cmd.DIRECT, Direct.CGADDR)

class SetDDRAMAddr(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(SetDDRAMAddr, cls).parse(packet)
        return arg0
    
SetDDRAMAddr.register(Cmd.DIRECT, Direct.DDADDR)

class Wait(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(Wait, cls).parse(packet)
        return arg0

Wait.register(Cmd.DIRECT, Direct.BUSY_WAIT)

class Write(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(Write, cls).parse(packet)
        return arg0

Write.register(Cmd.DIRECT, Direct.WRITE)

class Read(Reply):
    @classmethod
    def parse(cls, packet):
        arg0, arg1 = super(Read, cls).parse(packet)
        return arg0

Read.register(Cmd.DIRECT, Direct.READ)
