---
layout: posts
title: "NeoPixels on Arduino Uno, Part 1"
date: "2014-09-13"
categories: software
---

# Goals

The goal is to wire feedback from the I2C bus from the Pixhawk to the Uno to control the lighting strip on the quad.

Project is located here: [NeoPixels](https://github.com/nwind21/apm_neopixels)

# First Step

All test code will be use Google Mock and Google Test. All testing will be done on the host.

* VisualStudio 2013 used for the unit test harness
* AtmelStudio 6.2 used to xcompile for the Arduino Board

Pure interfaces classes will be written to abstract the Arduino code from the lighting control algorithm. Two classes have been created:

* II2C
* ILighting

Tests will focus enforcing the calling of the I2C and ILighting objects interfaces. Actual implementation of the I2C and ILighting layer will be tested on the hardware. The interfaces will stay as close to the hardware as possible to minimize the amount of physical hardware testing required and therefore permitting as much of the decision logic to be tested in the host framework. The interfaces will also not expose implementation details, e.g. the interfaces should not make assumptions of the underlying hardware (in the event, it changes).

# Next Step

After the interfaces and testing stubs are created I'll write up a basic lighting control object that runs through the interfaces to make sure the mocks are invoked on a generic lighting control before implementing the real logic.

I'm also considering a simple lightweight messaging bus that will allow me to connect arbitrary events for additional flexibility on what is triggered, versus hardcoding the handling of a trigger. I'm not quite 100% convinced the added complexity is worth it, but it'll be on the table.
