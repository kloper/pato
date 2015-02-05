# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/transport/uart.py -- pyserial transport for pato
#

import serial
import types

from util.protocol import ProtocolException

class Uart(object):
    def __init__(self, *args, **kwargs):
        self.serial = serial.Serial(*args, **kwargs)

    def query(self, request):
        if not isinstance(request, types.StringType):
            request = "".join(chr(c) for c in request)
        bytes_written = self.serial.write(request)

        if bytes_written != len(request):
            raise ProtocolException("Failed to send request")
        
        reply_size = 5
        reply = self.serial.read(reply_size)
        if len(reply) != reply_size:
            raise ProtocolException("Failed to receive reply")

        reply = [ord(c) for c in reply]
        return reply

    def close(self):
        self.serial.close()
        

