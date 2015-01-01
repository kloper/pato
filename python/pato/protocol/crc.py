# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/protocol/crc.py -- crc8 calculator for Pato packets
#

import os
import sys

def _crc_ibutton_update(crc, data):
    crc = (crc & 0xff) ^ (data & 0xff)
    for _ in xrange(8):
        if crc & 1:
            crc = (crc >> 1) ^ 0x8c
        else:
            crc >>= 1
    return crc

def crc8(data):
    crc = 0
    for c in data:
        crc = _crc_ibutton_update(crc, c)
    return crc
