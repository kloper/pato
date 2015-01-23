# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/bridge/protocol/__init__.py -- bridge driver
#

import os
import sys

class Cmd:
    PING = 1
    TWI_CONFIG = 2	 
    TWI_MASTER_SEND = 3
    TWI_MASTER_RECV = 4

class Error:
    COBS = 0x01
    CRC = 0x02
    BADCMD = 0x03
    BADARG = 0x04
