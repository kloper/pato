# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/util/protocol.py -- basic stuff for protocol definitions
#

class ProtocolException(Exception):
    def __init__(self, *args, **kwargs):
        args += (kwargs,)
        Exception.__init__(self, *args)
