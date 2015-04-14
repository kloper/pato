# -*- python -*-
"""@file

@brief crc8 calculator for Pato packets

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

def _crc_update(crc, data, mask, const):
    """
    CRC8/16 update function taken from _crc_ibutton_update() function
    found in "Atmel Toolchain/AVR8 GCC/Native/3.4.1061/
    avr8-gnu-toolchain/avr/include/util/crc16.h" documentation.
    @param[in] crc current CRC value
    @param[in] data next byte of data
    @param[in] mask CRC mask, must be 0xff for CRC8 and 0xffff for CRC16
    @param[in] const CRC polynomial constant
    @returns Next CRC value either 8 or 16 bit depending on mask
    """
    crc = (crc & mask) ^ (data & 0xff)
    for _ in range(8):
        if crc & 1:
            crc = (crc >> 1) ^ const
        else:
            crc >>= 1
    return crc & mask

def crc8(data):
    """
    Calculate CRC8 as described in the util/crc16.h (see _crc_update())
    @param[in] data input byte sequence
    @returns Optimized Dallas (now Maxim) iButton 8-bit CRC
    """
    crc = 0
    for c in data:
        crc = _crc_update(crc, c, 0xff, 0x8c)
    return crc

def crc16(data):
    """
    Calculate CRC16 as described in the util/crc16.h (see _crc_update())
    @param[in] data input byte sequence
    @returns CRC16 of input byte sequence
    """
    crc = 0
    for c in data:
        crc = _crc_update(crc, c, 0xffff, 0xA001)
    return crc
