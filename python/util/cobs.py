# -*- python -*-
"""@file

@brief Consistent Overhead Byte Stuffing
See http://stuartcheshire.org/papers/COBSforToN.pdf

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

def seq_split(seq, val):
    res = []
    for item in seq:
        if item == val:
            yield res
            res = []
        else:
            res.append(item)
    yield res

def seq_join(seq, val):
    res = []
    for item in seq[:-1]:
        res.extend(item)
        if val is not None:
            res.append(val)
    res.extend(seq[-1])
    return res

def encode(seq):
    assert len(seq) < 255, "COBS content too long"
    assert len(seq) > 0, "COBS content too short"
    return seq_join( [[len(s)+1]+s for s in seq_split(seq, 0)], None )

def decode(seq):
    assert len(seq) > 0, "COBS content too short"
    res = []
    while seq:
        count = seq[0]
        if count == 0:
            break
        res.append(seq[1:count])
        seq = seq[count:]
    return seq_join(res, 0)
