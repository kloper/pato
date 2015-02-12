# -*- python -*-
"""@file

@brief Bridge driver

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
                                                
