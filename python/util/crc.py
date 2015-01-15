# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/util/protocol/crc.py -- crc8 calculator for Pato packets
#

import os
import sys

def _crc_update(crc, data, mask, const):
    crc = (crc & mask) ^ (data & 0xff)
    for _ in xrange(8):
        if crc & 1:
            crc = (crc >> 1) ^ const
        else:
            crc >>= 1
    return crc & mask

def crc8(data):
    crc = 0
    for c in data:
        crc = _crc_update(crc, c, 0xff, 0x8c)
    return crc

def crc16(data):
    crc = 0
    for c in data:
        crc = _crc_update(crc, c, 0xffff, 0xA001)
    return crc
