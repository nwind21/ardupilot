---
title: SITL Refactor to use ZMQ
layout: posts
categories: software
---

# Mr. Autotest

When I first started this project I could not, for the life of me, get SITL to run consistently in autotest (this is the prescribed system level regression).  I started very shallow, first trying to see it was my environment, double-checking online documentation, rebuilding many times.  I still could not get autotest to reproduce results as everyone on the forums seem to say was working.

## The problems

I came across the following issues:

* <strong>Autotest would timeout.</strong>  Autotest builds everything and builds everything several times over.  I mean EVERYTHING.  I modified autotest to allow incremental builds and also fragment up the build.  I know that autotest allows you to specify "skips" but it was way too hard for me to remember what to skip every single time.
* <strong>SITL fails intermittently.</strong>  This was the most frustrating part.  After reducing the autotest build to a manageable level I had to eventually reverse engineer the SITL.  I fixed this by refactoring the communication core with ZMQ.  This leads me to the next discussion.

# SITL analysis

Conceptually, this is pretty easy to understand once you look at the code.  The physical simulation is in Python and is responsible for taking a PWM input, processing it, then sending back FDM packets which tell the SITL backend what the physical frame is doing (accel, gps, lat, lon, etc).  The SITL then processes the FDM packets and updates the sensors so that when the APM requests for data, the data is meaningful.

## Which leads me to the problems...

The assumption I began to make is that the FDM packets must always reflect the "real-world" simulation.  If the FDM packets are dropped, lagged, then the PWM feedback will be incorrect causing a feedback loop.  This was actually visible during my SITL debugging where the quad would fly in a circle trying to land.

With this assumption I made the following conclusions:

* FDM packets cannot be delayed.
* PWM packets cannot be tied to the FDM sending.
* FDM packets can be sent at steady stream, rate control isn't really necessary.  In fact you want the FDM data to be more relevant since that means the PWM packets will reflect the relevance of the FDM packats.
* The SITL must process the FDM packets timely such that the APM code can send proper PWM packets back to the python sim for motor control.  This means the SITL back-end needed to be extremely lag free otherwise another feedback loop could develop on the SITL side.

## The Old SITL

SITL runs a high speed 1khz real-time timer to trigger pretty much everything.  This timer is VERY sensitive and responsible for:

* Servicing FDM packets.
* Updating sensor data.
* Sending PWM packets.
* Triggering anything waiting on the timers.

As you can see, the timer loop has a lot riding on it.  So let's change this!

## The New SITL

I turned to [ZeroMQ](http://zeromq.org/).

To decouple all the logic riding on this timer loop I made the following refactor decisions:

* I decided the SITL back-end would use ZMQ publisher and subscriber sockets to communicate.  This would allow the data to flow freely and let ZMQ handle the connection details as well as the details.  This also means I can add more subscribers later if I wanted to capture the FDM or PWM data for other logging.
* I decided to thread each of the SITL sensor update.  This made sense since the code already took an FDM then sent the packets to individual update_XYZ functions.  Perfect.  Thread the update_XYZ so they can update as the data arrives!
*  I also threaded the python code.  The FDM publisher is a steady sender that just packages the frame details.  The PWM subscriber would block waiting for PWM data.

## The diagram

So here's the before and after.  The lines try to illustrate the poll loops.  In the second diagram, notice how the timer loop doesn't service much any more.

<img src="https://dl.dropboxusercontent.com/u/4242148/Blog/ZeroMQSITL.png" class="img-thumbnail"/>
