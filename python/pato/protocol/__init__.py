# -*- python -*-
"""@file

@brief Basic stuff for communication with Pato

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

import os
import sys

class Cmd:
    PING = 0x00
    DIRECT = 0x01
    RESET = 0x02
    CONTRAST = 0x03
    BACKLIGHT = 0x04
    PRINT_SETADDR = 0x05
    PRINT_GETADDR = 0x06
    PRINT_PUT = 0x07
    PRINT_COMMIT = 0x08
    
class Direct:
    CLR = 0x01
    HOME = 0x02
    EMS = 0x03
    DCTRL = 0x04
    SHIFT = 0x05
    FUNC = 0x06
    CGADDR = 0x07
    DDADDR = 0x08
    BUSY_WAIT = 0x09
    WRITE = 0x0A
    READ = 0x0B

class Reply:
    OK = 0x00
    ERROR = 0xff

class Error:
    CRC = 0x01
    BADCMD = 0x02
    BADARG = 0x03
