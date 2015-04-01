# -*- python -*-
"""@file

@brief Basic stuff for constructing Pato replies

Copyright (c) 2014-2015 Dimitry Kloper <kloper@users.sf.net>. 
All rights reserved.

@page License

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation
are those of the authors and should not be interpreted as representing
official policies, either expressed or implied, of the Pato Project.
"""

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

class PrintPutPtr(Reply):
    @classmethod
    def parse(cls, packet):
        rc, cmd = super(PrintPutPtr, cls).parse(packet)
        cls.assertTrue(cmd == Cmd.PRINT_PUT_PTR,
                       "Unexpected command in reply: {}".format(cmd))
        cls.assertTrue(rc != 0, "Unable to put into print buffer")
        return rc

PrintPutPtr.register(Cmd.PRINT_PUT_PTR)

class PrintCommit(Reply):
    @classmethod
    def parse(cls, packet):
        rc, cmd = super(PrintCommit, cls).parse(packet)
        cls.assertTrue(cmd == Cmd.PRINT_COMMIT,
                       "Unexpected command in reply: {}".format(cmd))
        return rc
    
PrintCommit.register(Cmd.PRINT_COMMIT)
