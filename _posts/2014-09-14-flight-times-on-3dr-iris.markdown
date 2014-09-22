---
layout: posts
title: "Flight Times on 3DR Iris with Modifications"
date: "2014-09-14"
categories: hardware
---

It's reported in several times that the 3DR Iris flight time are overly optimistic.  I've mostly been reading the work that Christian Elsen has been doing @ [Cloud Surfer](http://cloud-surfer.net/2014/07/14/3dr-iris-with-4s-batteries/)

I'm tracking the flights times with my avionics so I can get a baseline before doing the modification for the 9x4.7 props and Mulistar 5100mAh battery which another 3DR flyer has found to work well.  Also I want to "burn" through these smaller batteries and get as much flight time before I retire them from the Iris.

# Current Frame

* Modified Frame inc Drive(with Uno + LEDs + Mast): 1460g
* Unmodified Frame inc Drive: 1280g

So my board, lights, zip ties, added about 180g which leaves me 220g left of working capacity (assuming the lift number isn't as optimistic as the flight times)

# Average Flight Times

I've begun to document the flight times here (Google Docs) @ [Flight Log ](https://docs.google.com/spreadsheet/ccc?key=0Aqyx1E1xb0wOdF9rTkpWazNmcWhNQWRSTHktQTA2SEE&usp=sharing)

In summary, I'm looking at about 6min average flight time which is not even close to the expected 10 minutes. This is flying in hover mode till the Iris executes a LAND on low battery and based on the timer on my radio as soon as throttle stick is moved.  This is also including powering two strips of lights and my Arduino.  I also did another 2 battery flight without any of the lights or board powered and the flight times didn't change much.

Also I noticed that the battery will hit 100% then fall-off to 95% capacity within an hour or so of charging finish.  I've recorded this data prior to flight to follow up later.

I was hoping to carry a NEX camera payload to do some photogrammetry, but with these flight times I have a feeling the load carrying capacity may be optimistic as well since any camera on the bottom will also need long legs and a chassis which will exceed the OEM configuration.

# References
You'll need a paid version of XCalc to see these values.  All that is changed is the weight for my modifications.  These values are courtesy of Elsen (he also has a link too, I think)

[3DR Iris XCalc Values](www.ecalc.ch/xcoptercalc.php?ecalc&lang=en&cooling=2.5&rotornumber=4&config=1&weight=1480&calc=auw&elevation=122&airtemp=27&qnh=1013&batteries=0&battcap=3500&battri=0.0052&battv=3.7&battccont=30&battcmax=40&battweight=63&chargestate=0&s=3&p=1&esc=max_20a&motor=rctimer&type=54|a2830-12&gear=1&propeller=apc_slowfly_sf&diameter=10&pitch=4.7&blades=2)
