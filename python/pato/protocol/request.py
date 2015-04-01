# -*- python -*-
"""@file

@brief Basic stuff for constructing Pato requests

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

from pato.protocol.packet import Request
from pato.protocol import Cmd, Direct

class Ping(Request):
    @classmethod
    def compile(cls, value):
        cls.assertTrue( value >= 0 and value <= 0xff,
                         "value must be in range [0..0xff]" )
        return super(Ping, cls).compile([0, value])

Ping.register(Cmd.PING)

class ClearScreen(Request):
    @classmethod
    def compile(cls):
        return super(ClearScreen, cls).compile([Direct.CLR, 0])
    
ClearScreen.register(Cmd.DIRECT, Direct.CLR)

class Home(Request):
    @classmethod
    def compile(cls):
        return super(Home, cls).compile([Direct.HOME, 0])
    
Home.register(Cmd.DIRECT, Direct.HOME)

class EntryModeSet(Request):
    @classmethod
    def compile(cls, shift_direction,  shift_subject):
        param = (2 if shift_direction else 0) | (1 if shift_subject else 0)
        return super(EntryModeSet, cls).compile([Direct.EMS, param])
    
EntryModeSet.register(Cmd.DIRECT, Direct.EMS)

class DisplayControl(Request):
    @classmethod
    def compile(cls, display_on,  cursor_on, cursor_blink):
        param = (4 if display_on else 0) | \
                (2 if cursor_on else 0) | \
                (1 if cursor_blink else 0)
        return super(DisplayControl, cls).compile([Direct.DCTRL, param])
    
DisplayControl.register(Cmd.DIRECT, Direct.DCTRL)

class Shift(Request):
    @classmethod
    def compile(cls, display_shift,  right_shift):
        param = (8 if display_shift else 0) | \
                (4 if right_shift else 0)
        return super(Shift, cls).compile([Direct.Shift, param])
    
Shift.register(Cmd.DIRECT, Direct.SHIFT)

class FunctionSet(Request):
    @classmethod
    def compile(cls, wide_bus, two_lines, large_font):
        param = (16 if wide_bus else 0) | \
                (8 if two_lines else 0) | \
                (4 if large_font else 0)
        return super(FunctionSet, cls).compile([Direct.FUNC, param])
    
FunctionSet.register(Cmd.DIRECT, Direct.FUNC)

class SetCGRAMAddr(Request):
    @classmethod
    def compile(cls, addr):
        return super(SetCGRAMAddr, cls).compile([Direct.CGADDR, addr & 0x3F])
    
SetCGRAMAddr.register(Cmd.DIRECT, Direct.CGADDR)

class SetDDRAMAddr(Request):
    @classmethod
    def compile(cls, addr):
        return super(SetDDRAMAddr, cls).compile([Direct.DDADDR, addr & 0x7F])
    
SetDDRAMAddr.register(Cmd.DIRECT, Direct.DDADDR)

class Wait(Request):
    @classmethod
    def compile(cls):
        return super(Wait, cls).compile([Direct.BUSY_WAIT, 0])
    
Wait.register(Cmd.DIRECT, Direct.BUSY_WAIT)

class Write(Request):
    @classmethod
    def compile(cls, val):
        return super(Write, cls).compile([Direct.WRITE, val & 0xff])
    
Write.register(Cmd.DIRECT, Direct.WRITE)

class Read(Request):
    @classmethod
    def compile(cls):
        return super(Read, cls).compile([Direct.READ, 0])
    
Read.register(Cmd.DIRECT, Direct.READ)

class Reset(Request):
    @classmethod
    def compile(cls, param, value = 0):
        cls.assertTrue( param in [0,1,2], "Reset parameter must be 0, 1 or 2")
        return super(Reset, cls).compile([param, value])
    
Reset.register(Cmd.RESET)

class PrintSetAddr(Request):
    @classmethod
    def compile(cls, addr):
        return super(PrintSetAddr, cls).compile([addr & 0xff,
                                                 (addr >> 8) & 0xff])
    
PrintSetAddr.register(Cmd.PRINT_SETADDR)

class PrintGetAddr(Request):
    @classmethod
    def compile(cls):
        return super(PrintGetAddr, cls).compile([0, 0])
    
PrintGetAddr.register(Cmd.PRINT_GETADDR)

class PrintPut(Request):
    @classmethod
    def compile(cls, b0, b1):
        return super(PrintPut, cls).compile([b0 & 0xff, b1 & 0xff])
    
PrintPut.register(Cmd.PRINT_PUT)

class PrintPutPtr(Request):
    @classmethod
    def compile(cls, offset):
        return super(PrintPutPtr, cls).compile([offset & 0xff,
                                                (offset >> 8) & 0xff])
    
PrintPutPtr.register(Cmd.PRINT_PUT_PTR)

class PrintCommit(Request):
    @classmethod
    def compile(cls, offset):
        return super(PrintCommit, cls).compile([offset & 0xff,
                                                (offset >> 8) & 0xff])

PrintCommit.register(Cmd.PRINT_COMMIT)
