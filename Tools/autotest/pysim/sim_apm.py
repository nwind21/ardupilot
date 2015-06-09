#!/usr/bin/env python
from __future__ import print_function
import time
import argparse
import signal
import zmq
import socket, struct

# Globals
dataCnt = 0

parser = argparse.ArgumentParser( description= 'Load in data and feed to simulation.' )
parser.add_argument( '--infile',
                     required=True )
parser.add_argument( '--freq',
                     required=True )

args = parser.parse_args( )
freqInFloat = float(args.freq)
print( "[FREQ] " + str( freqInFloat ) )

def signal_handler(signum, frame):
    socket_pub_pwm.close()
    socket_sub_fdm.close()
    zmqContext.term()
    print( "[" + str(dataCnt) + "] Data Packets          " )
    exit( 0 )

signal.signal(signal.SIGINT, signal_handler)

# Open PWM port
# Global ZMQ Context, this is thread safe to pass around.
zmqContext = zmq.Context()

socket_pub_pwm = zmqContext.socket( zmq.PUB )
socket_pub_pwm.bind( "tcp://*:5502" )

socket_sub_fdm = zmqContext.socket( zmq.SUB )
socket_sub_fdm.connect( 'tcp://localhost:5503' )
socket_sub_fdm.setsockopt( zmq.SUBSCRIBE,
                           b"" )

with open( args.infile, 'rb') as f:

    while True:
        print( "[" + str(dataCnt) + "] Data Packets          ", end='\r' )
        bytes = f.read( 28 )
        if bytes == '':
            break
        dataCnt += 1
        socket_pub_pwm.send( bytes, zmq.NOBLOCK )
        time.sleep( freqInFloat )

socket_pub_pwm.close()
socket_sub_fdm.close()
zmqContext.term()
print( "[" + str(dataCnt) + "] Data Packets" )
