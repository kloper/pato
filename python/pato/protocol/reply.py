# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/protocol/reply.py -- basic stuff for constructing Pato replys
#

import pdb

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
        return (arg0, arg1)

Read.register(Cmd.DIRECT, Direct.READ)

class Reset(Reply):
    @classmethod
    def parse(cls, packet):
        rc, cmd = super(Reset, cls).parse(packet)
        cls.assertTrue(cmd == Cmd.RESET,
                       "Unexpected command in reply: {}".format(cmd))
        return rc

Reset.register(Cmd.RESET)

class PrintSetAddr(Reply):
    @classmethod
    def parse(cls, packet):
         rc, cmd = super(PrintSetAddr, cls).parse(packet)
         cls.assertTrue(cmd == Cmd.PRINT_SETADDR,
                        "Unexpected command in reply: {}".format(cmd))
         cls.assertTrue(rc != 0, "Unable to set print buffer address")
         return rc
    
PrintSetAddr.register(Cmd.PRINT_SETADDR)

class PrintGetAddr(Reply):
    @classmethod
    def parse(cls, packet):
        b0, b1 = super(PrintGetAddr, cls).parse(packet)
        return ((b1 << 8) | b0)
    
PrintGetAddr.register(Cmd.PRINT_GETADDR)

class PrintPut(Reply):
    @classmethod
    def parse(cls, packet):
        rc, cmd = super(PrintPut, cls).parse(packet)
        cls.assertTrue(cmd == Cmd.PRINT_PUT,
                       "Unexpected command in reply: {}".format(cmd))
        cls.assertTrue(rc != 0, "Unable to put into print buffer")
        return rc
    
PrintPut.register(Cmd.PRINT_PUT)

class PrintCommit(Reply):
    @classmethod
    def parse(cls, packet):
        rc, cmd = super(PrintCommit, cls).parse(packet)
        cls.assertTrue(cmd == Cmd.PRINT_COMMIT,
                       "Unexpected command in reply: {}".format(cmd))
        return rc
    
PrintCommit.register(Cmd.PRINT_COMMIT)
