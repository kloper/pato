# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/pato/__init__.py -- Pato driver
#

from util.protocol import ProtocolException

from pato.protocol.request import Request
from pato.protocol.reply import Reply
from pato.protocol import Cmd

class Pato(object):
    def __init__(self, transport):
        self.transport = transport

    def execute(self, cmd, *args, **kwargs):
        if cmd != Cmd.DIRECT:
            request_class = Request.find(cmd)
            reply_class = Reply.find(cmd)
        else:
            request_class = Request.find(cmd, args[0])
            reply_class = Reply.find(cmd, args[0])
            args = args[1:]
        assert request_class is not None, \
            "Can't find request template for  command: {}".format(cmd)
        assert reply_class is not None, \
            "Can't find reply template for command : {}".format(cmd)

        request = request_class.compile(*args, **kwargs)
        bytes_written = self.transport.write("".join(chr(c) for c in request))

        if bytes_written != len(request):
            raise ProtocolException("Failed to send request")
        
        reply_size = 5
        reply = self.transport.read(reply_size)
        if len(reply) != reply_size:
            raise ProtocolException("Failed to receive reply")
        
        result = reply_class.parse([ord(c) for c in reply])

        return result
                                                
