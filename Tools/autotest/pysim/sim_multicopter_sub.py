#!/usr/bin/env python
from __future__ import print_function
import time
import signal
import zmq
import socket, struct

# Global
cnt = 0
print( "[START] Listening for simulation frame data...")

# Open PWM port
# Global ZMQ Context, this is thread safe to pass around.
zmqContext = zmq.Context()

socket_sub_fdm = zmqContext.socket( zmq.SUB )
socket_sub_fdm.connect( 'tcp://localhost:5503' )
socket_sub_fdm.setsockopt( zmq.SUBSCRIBE,
                           b"" )

def signal_handler(signum, frame):
    print( "[" + str(cnt) + "] Received packet " )
    socket_sub_fdm.close()
    zmqContext.term()
    exit( 0 )

signal.signal(signal.SIGINT, signal_handler)

'''receive control information from SITL'''
lastLat = 0
lastLon = 0
lastAlt = 0
while True:
    try:
        buf = socket_sub_fdm.recv( 140 )
        cnt += 1
    except zmq.ZMQError, e:
        if e.errno == zmq.EAGAIN:
            time.sleep( 0.01 )
            continue
        else:
            raise e

    if len(buf) != 140 :
        print( "[EXIT] Error, buffer length we expected 17*4 but got something else." + str(len(buf)))
        exit( 1 )
    unpacked_data = struct.unpack( '<17dI',
                                   buf )
    if lastLat != unpacked_data[0] or lastLon != unpacked_data[1] or lastAlt != unpacked_data[2]:
        print( unpacked_data[0], unpacked_data[1], unpacked_data[2] )
    lastLat = unpacked_data[0]
    lastLon = unpacked_data[1]
    lastAlt = unpacked_data[2]
