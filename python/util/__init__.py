# -*- python -*-
"""@file

@brief Common utilies

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

import struct

def pairs(strn):
    """
    Generator that splits given string into sequence of pairs
    of characters. If string contains odd number of characters
    it is padded with 0 chars.

    @param[in] strn string to be split into pairs
    @returns on each invocation returns next pair of characters
            from the string.
    """
    strlen = len(strn) + len(strn)%2
    strn += "\0"
    for i in range(0, strlen, 2):
        yield strn[i:i+2]

def int2str(val):
    """
    Convert unsigned short into little endian byte sequence
    @param[in] val integer value
    @returns byte string representing unsigned short
    """
    return struct.pack("<H", val)

def float2str(val):
    """
    Convert float value into little endian byte sequence
    @param[in] val float value
    @returns byte string representing float value
    """
    return struct.pack("<f", float(val))

builtin_range = range #pylint: disable=invalid-name
range = builtin_range #pylint: disable=invalid-name,redefined-builtin

try:
    xrange #pylint: disable=pointless-statement,undefined-variable
    range = xrange #pylint: disable=undefined-variable
except NameError:
    pass
