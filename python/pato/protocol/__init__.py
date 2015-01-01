# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/protocol/__init__.py -- basic stuff for communication with Pato
#

import os
import sys

class Cmd:
    PING = 0x00
    DIRECT = 0x01
    RESET = 0x02
    CONTRAST = 0x03
    BACKLIGHT = 0x04

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

class ProtocolException(Exception):
    def __init__(self, *args, **kwargs):
        args += (kwargs,)
        Exception.__init__(self, *args)
