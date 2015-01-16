# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/bridge/__init__.py -- bridge driver
#

import pdb

from util.protocol import ProtocolException

from bridge.protocol.request import Request
from bridge.protocol.reply import Reply
from bridge.protocol import Cmd

class Bridge(object):
    def __init__(self, transport):
        self.transport = transport
    
    def execute(self, cmd, *args, **kwargs):
        request_class = Request.find(cmd)
        reply_class = Reply.find(cmd)
        assert request_class is not None, \
            "Can't find request template for  command: {}".format(cmd)
        assert reply_class is not None, \
            "Can't find reply template for command : {}".format(cmd)

        request = request_class.compile(*args, **kwargs)
        bytes_written = self.transport.write("".join(chr(c) for c in request))

        if bytes_written != len(request):
            raise ProtocolException("Failed to send request")
        
        reply_size = 4
        head = self.transport.read(reply_size)
        if len(head) != reply_size:
            raise ProtocolException("Failed to receive reply")
        reply_size = ord(head[1])+1
        body = self.transport.read(reply_size)
        if len(body) != reply_size:
            raise ProtocolException("Failed to receive reply")
        result = reply_class.parse([ord(c) for c in head+body])

        return result
                                                
