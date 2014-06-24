---
title: Avionics Plate Part 2
layout: posts
---

# The Damage

This project took way longer than I thought.  I had to source parts from Hobby King, local hobby store, hardware store for specialty screws, electronics store, Amazon.com, Ardafruit, 3DR Robotics and Ebay.  Whew!  All for fancy lights!

The total amperage draw from this system is less than 1.2A.  I can potentially hit 1.2A if I go full brightness, white but that will rarely happen, if ever.  I have the I2C wired but not attached.

## Work List

* 50+ solder joints including tinning wires and pads
* Zip-ties galore
* Designing and laser-cutting the avionics plate
* Slow and painful routing of wires through the Iris case.
* Drilling the case and the Iris avionics plate.
* Hot glue for stress relief of soldering joints.
* Taking apart the frame probably over 10x.

## Part List

* 22 AWG wire for joining NeoPixels.
* 18 AWG silicon wire for power lines to the voltage regulator.
* 4x M3 Nylon standoffs that will tall enough to go through shell (>20mm)
* 8x M3 Nylon screws
* 4x 4x40 Nylon spacers for Arduino Board
* 8x 4x40 Allen keyed screws (the nylon screws heads were too big) for Arduino Board.  I believe four were 4x40 1/4" and the other four were 4x40 3/16".
* Handful of JST male and female connectors for power.
* Jumper wires for control lines for the NeoPixels.
* 1000uF 10v capacitor for NeoPixels.
* 4x 300ohm resistors for each NeoPixel signal, soldered inline.
* Step-Down voltage regulator to provide 5V 3.5A from Gimbal Power tap.
* Female Banana plugs in case I want to power the Uno and lights without using powering the entire quad.  ( Battery -> XT60 -> Banana Plugs -> JST -> Step-Down Regulator )
* GPS Mast from 3DR
* DF3 4-pin pigtail connector.  I bought a long one from 3DR, cut it, then spliced in jumper wires for the I2C.

# The Assembly

Here are a few photos of the final assembly.  I'm glad to be stopping this little hardware project to return back to some coding!

## Wire route

Wires follow the motor wires through the shell them rise up through the inside and surface through the top shell.  Power wires are tucked with the 4in1 ESC and the capacitor hangs out near the battery compartment door.

<img src="https://dl.dropboxusercontent.com/u/4242148/Blog/BottomWiring.jpg" class="img-thumbnail"/>

Battery compartment view

<img src="https://dl.dropboxusercontent.com/u/4242148/Blog/BatteryCompartment.jpg" class="img-thumbnail"/>

## Standoffs

Variety of standoffs and connectors.

<img src="https://dl.dropboxusercontent.com/u/4242148/Blog/Standoffs.jpg" class="img-thumbnail"/>

## Power

Voltage regulator, silicon wire.  Nothing too fancy here.

<img src="https://dl.dropboxusercontent.com/u/4242148/Blog/PowerRegulator.jpg" class="img-thumbnail"/>