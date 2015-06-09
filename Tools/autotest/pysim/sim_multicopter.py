#!/usr/bin/env python

from multicopter import MultiCopter
import util, time, os, sys, math
import socket, struct
import select, errno
import signal
import zmq
from threading import Thread

from pymavlink import fgFDM

def sim_send(fd, m, a):
    '''send flight information to mavproxy and flightgear'''
    global fdm
    from math import degrees

    earth_rates = util.BodyRatesToEarthRates(a.dcm, a.gyro)
    (roll, pitch, yaw) = a.dcm.to_euler()

    '''
    fdm.set('latitude', a.latitude, units='degrees')
    fdm.set('longitude', a.longitude, units='degrees')
    fdm.set('altitude', a.altitude, units='meters')
    fdm.set('phi', roll, units='radians')
    fdm.set('theta', pitch, units='radians')
    fdm.set('psi', yaw, units='radians')
    fdm.set('phidot', earth_rates.x, units='rps')
    fdm.set('thetadot', earth_rates.y, units='rps')
    fdm.set('psidot', earth_rates.z, units='rps')
    fdm.set('vcas', math.sqrt(a.velocity.x*a.velocity.x + a.velocity.y*a.velocity.y), units='mps')
    fdm.set('v_north', a.velocity.x, units='mps')
    fdm.set('v_east', a.velocity.y, units='mps')
    # FG FDM protocol only supports 4 motors for display :(
    fdm.set('num_engines', 4)
    for i in range(4):
        fdm.set('rpm', 1000*m[i], idx=i)
    try:
        fg_out.send(fdm.pack())
    except socket.error as e:
        if not e.errno in [ errno.ECONNREFUSED ]:
            raise
    '''

    buf = struct.pack('<17dI',
                      a.latitude, a.longitude, a.altitude, degrees(yaw),
                      a.velocity.x, a.velocity.y, a.velocity.z,
                      a.accelerometer.x, a.accelerometer.y, a.accelerometer.z,
                      degrees(earth_rates.x), degrees(earth_rates.y), degrees(earth_rates.z),
                      degrees(roll), degrees(pitch), degrees(yaw),
                      math.sqrt(a.velocity.x*a.velocity.x + a.velocity.y*a.velocity.y),
                      0x4c56414f)
    try:
        fd.send(buf, zmq.NOBLOCK)
    except socket.error as e:
        if not e.errno in [ errno.ECONNREFUSED ]:
            raise


def sim_recv(fd, m, out=None):
    '''receive control information from SITL'''
    try:
        buf = fd.recv(28)
    except socket.error as e:
        if not e.errno in [ errno.EAGAIN, errno.EWOULDBLOCK ]:
            print( e.errno )
            raise
        return False

    if len(buf) != 28 :
        return False

    # Log to file
    if out:
        out.write( buf )

    control = list(struct.unpack('<14H', buf))
    pwm = control[0:11]

    # update motors
    for i in range(11):
        m[i] = (pwm[i]-1000)/1000.0

    # update wind
    global a
    (speed, direction, turbulance) = control[11:]
    a.wind.speed = speed*0.01
    a.wind.direction = direction*0.01
    a.wind.turbulance = turbulance*0.01
    return True


def interpret_address(addrstr):
    '''interpret a IP:port string'''
    a = addrstr.split(':')
    a[1] = int(a[1])
    return tuple(a)

##################
# main program
from optparse import OptionParser
parser = OptionParser("sim_multicopter.py [options]")
parser.add_option("--fgout", dest="fgout",  help="flightgear output (IP:port)", default="127.0.0.1:5505")
parser.add_option("--simin",  dest="simin",   help="SIM input (IP:port)",       default="localhost:5502")
parser.add_option("--simout", dest="simout",  help="SIM output (port)",      default="5503")
parser.add_option("--home", dest="home",  type='string', default=None, help="home lat,lng,alt,hdg (required)")
parser.add_option("--wind", dest="wind", help="Simulate wind (speed,direction,turbulance)", default='0,0,0')
parser.add_option("--frame", dest="frame", help="frame type (+,X,octo)", default='+')

(opts, args) = parser.parse_args()

for m in [ 'home' ]:
    if not opts.__dict__[m]:
        print("[SIM] Missing required option '%s'" % m)
        parser.print_help()
        sys.exit(1)

# UDP socket addresses
fg_out_address  = interpret_address(opts.fgout)
sim_in_address  = interpret_address(opts.simin)

# setup output to flightgear
fg_out = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
fg_out.connect(fg_out_address)
fg_out.setblocking(0)

# Global ZMQ Context, this is thread safe to pass around.
zmqContext = zmq.Context()

# FG FDM object
fdm = fgFDM.fgFDM()

# create the quadcopter model
a = MultiCopter(frame=opts.frame)

print("[SIM] Simulating %u motors for frame %s" % (len(a.motors), opts.frame))

# motors initially off
m = [0.0] * 11

# parse home
v = opts.home.split(',')
if len(v) != 4:
    print("[SIM] Home should be lat,lng,alt,hdg")
    sys.exit(1)
a.home_latitude = float(v[0])
a.home_longitude = float(v[1])
a.home_altitude = float(v[2])
a.latitude = a.home_latitude
a.longitude = a.home_longitude
a.altitude = a.home_altitude
a.yaw = float(v[3])
a.ground_level = a.home_altitude
a.position.z = 0
a.wind = util.Wind(opts.wind)
a.set_yaw_degrees(a.yaw)

print("[SIM] Starting at lat=%f lon=%f alt=%.1f heading=%.1f" % (
    a.home_latitude,
    a.home_longitude,
    a.home_altitude,
    a.yaw))

def receiveThread( context ):
    f = None
    #f = open( 'sim_in.bin', 'wb')
    address = 'tcp://' + sim_in_address[0] + ":" + str(sim_in_address[1])
    print( "[SIM] receiveThread up!" )
    print( "[SIM] receiveThread connecting publisher @ " + address + ":" )
    sim_in = context.socket( zmq.SUB )
    sim_in.connect(address)
    sim_in.setsockopt(zmq.SUBSCRIBE, b'')

    sim_kill = context.socket( zmq.SUB )
    sim_kill.connect('inproc://kill')
    sim_kill.setsockopt(zmq.SUBSCRIBE, b"")

    sim_sync = context.socket( zmq.REQ )
    sim_sync.connect('inproc://sync')

    # Send up signal
    sim_sync.send( b'' )
    sim_sync.recv( )

    poll = zmq.Poller()
    poll.register(sim_kill, zmq.POLLIN)
    poll.register(sim_in,  zmq.POLLIN)

    cnt = 0
    while True:
        sockets = dict(poll.poll())
        if sockets.get(sim_in) == zmq.POLLIN:
            if sim_recv(sim_in, m, f) == True:
                cnt += 1
            m2 = m[:]
            a.update(m2)
        if sockets.get(sim_kill) == zmq.POLLIN:
            sim_in.close()
            sim_kill.close()
            break

    # f.close()
    print( "[SIM] receiveThread, " + str(cnt) + "p " )
    print( "[SIM] receiveThread is exiting" )
    sim_sync.send( b'' )
    sim_sync.recv( )
    print( "[SIM] SIGKILL received, receiveThread" )

def sendThread( context ):
    address = 'tcp://*:' + opts.simout
    print( "[SIM] sendThread up!" )
    print( "[SIM] sendThread binding as publisher @ " + address )
    sim_out = context.socket( zmq.PUB )
    sim_out.bind( address )

    sim_kill = context.socket( zmq.SUB )
    sim_kill.connect('inproc://kill')
    sim_kill.setsockopt(zmq.SUBSCRIBE, b"")

    sim_sync = context.socket( zmq.REQ )
    sim_sync.connect('inproc://sync')

    # Send up signal
    sim_sync.send( b'' )
    sim_sync.recv( )

    poll = zmq.Poller()
    poll.register(sim_kill, zmq.POLLIN)
    poll.register(sim_out,  zmq.POLLOUT)

    cnt = 0
    loopcnt  = 0
    logstart = time.time()
    while True:
        sockets = dict(poll.poll())
        if sockets.get(sim_out) == zmq.POLLOUT:
            sim_send(sim_out, m, a)
            cnt += 1
        if sockets.get(sim_kill) == zmq.POLLIN:
            sim_out.close()
            sim_kill.close()
            break
        # 100Hz
        time.sleep( 0.01 )
        loopcnt += 1

    logend = time.time()
    print( "[SIM] sendThread, " + str(cnt) + "p, " + str(cnt/(logend-logstart)) + "p/s, l:" + str(loopcnt) )
    print( "[SIM] sendThread is exiting" )
    sim_sync.send( b'' )
    sim_sync.recv( )
    print( "[SIM] SIGKILL received, sendThread" )


sim_kill = zmqContext.socket( zmq.PUB )
sim_kill.bind( "inproc://kill" )

sim_sync = zmqContext.socket( zmq.REP )
sim_sync.bind( "inproc://sync" )

def signal_handler(signum, frame):
    print( "[SIM] Sending kill signal" )
    sim_kill.send( b"DIE" )
    print( "[SIM] Sent..." )

signal.signal(signal.SIGINT, signal_handler)

r = Thread( target=receiveThread, args=( zmqContext, ) )
r.start()
s = Thread( target=sendThread, args=( zmqContext, ) )
s.start()

# Send synchronization, we wait for 2 threads
threadCnt = 0
while threadCnt < 2:
    msg = sim_sync.recv()
    sim_sync.send(b'')
    threadCnt += 1
    print( "[SIM] +1 thread up." )

print( "[SIM] All threads up, entering main loop" )

# Main loop does nothing, wait for threads to die
while threadCnt > 0:
    print( "[SIM] Waiting for threads to stop." )
    try:
        msg = sim_sync.recv()
        sim_sync.send(b'')
        threadCnt -= 1
        print( "[SIM] -1 thread down." )
    except zmq.ZMQError as e:
        pass

time.sleep(1)
print( "[SIM] All threads down, exiting... " )
sim_kill.close()
sim_sync.close()
zmqContext.term()
print( "[SIM] Exited." )
exit( 0 )
