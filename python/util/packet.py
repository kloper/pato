# -*- python -*-
"""@file

@brief basic stuff for constructing protocols

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

from collections import defaultdict
from util.protocol import ProtocolException

def tree():
    """
    Tree definition from https://gist.github.com/hrldcpr/2012250
    """
    return defaultdict(tree)

class PacketBase(object):
    """
    Base class for constructing communication packets.
    Implements simple registration mechanism that allows to find specific
    handler classes according to packet contents.

    Sub-classes shall call static PacketBase::register() with some sort of
    unique id sequence to register themselves as handlers for that id.

    Later, the main application loop uses PacketBase::find() to find the
    appropriate handler for incoming or outgoing packets.
    """
    @classmethod
    def register(cls, *args):
        """
        Register class (this is @b static method so class object is registered
        not an instance of the class) object as handler for packets identified
        by args.
        @param[in] cls reference to the class object
        @param[in] args unique id sequence
        """
        assert len(args) > 0, "register expects one or more arguments"
        reg = cls.Registry #pylint: disable=no-member
        for cmd in args[0:-1]:
            reg = reg[cmd]
        assert args[-1] not in reg, \
          "Command with ID {} is already registered".format(args)
        reg[args[-1]] = cls
        cls.cmd = args[0]

    @classmethod
    def find(cls, *args):
        """
        Find class object that is registered for unique sequence.
        @param[in] cls reference to the base class object
        @param[in] args unique id sequence
        @returns Reference to specific class that is registered for that id.
                 None if nothing found.
        """
        assert len(args) > 0, "find expects one or more arguments"
        reg = cls.Registry #pylint: disable=no-member
        for cmd in args:
            if cmd in reg:
                reg = reg[cmd]
            else:
                return None
        return reg

    @staticmethod
    def assert_true(expr, *args, **kwargs):
        """
        Utility assert that will throw ProtocolException
        instead of AssertionError
        """
        if not expr:
            raise ProtocolException(*args, **kwargs)

    @staticmethod
    def assert_flase(expr, *args, **kwargs):
        """
        Utility assert that will throw ProtocolException
        instead of AssertionError
        """
        if expr:
            raise ProtocolException(*args, **kwargs)
