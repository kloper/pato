# -*- python -*-
#
# Copyright (c) Dimitry Kloper <kloper@users.sf.net> 2014-2015
#
# python/bridge/protocol/packet.py -- basic stuff for constructing
#                                     bridge packets
#

import pdb

import util.cobs as cobs

from util.packet import PacketBase, tree
from util.protocol import ProtocolException
from util.crc import crc16

class Request(PacketBase):
    Registry = tree()
    
    @classmethod
    def compile(cls, payload = []):
        res = [cls.cmd]+payload
        size = len(payload)+1
        crc = crc16(res)
        res = cobs.encode(
            [size & 0xff, crc & 0xff, (crc >> 8) & 0xff] + res
        ) + [0]
        return res

class Reply(PacketBase):
    Registry = tree()
    
    @classmethod
    def parse(cls, packet):        
        cls.assertTrue( packet[-1] == 0 and min(packet[:-1]) > 0,
                        "Packet is not COBS encoded" )

        packet = cobs.decode(packet)
        size = packet[0]
        cls.assertTrue( size == len(packet)-3,
                        msg = "Bad Size",
                        packet = packet)

        
        crc = (packet[2] << 8) | packet[1]
        cls.assertTrue( crc == crc16(packet[3:]),
                        msg = "Bad CRC",
                        packet = packet)

        cls.assertTrue( packet[3] > 0 ,
                         msg = "Error responce",
                         error_code = packet[5],
                         cmd = packet[4] )

        return (packet[3], packet[4:])
