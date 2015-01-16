# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/util/cobs.py --  Consistent Overhead Byte Stuffing
#
# See http://stuartcheshire.org/papers/COBSforToN.pdf
#

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
