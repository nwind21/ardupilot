---

title: Introduction
layout: default
icon: welcome
---

# A Quick Introduction

Hi, I'm Francis!  I graduated from UCSD in sunny San Diego and originally started my career in embedded software, working on high-speed cryptographic processors, wandered over to medical infusion pumps then, meandered into variety of roles dealing with video media while a doing a rather long stint @ [DivX Inc](http://www.divx.com).

I've been working in the software industry for over 16 years just kinda trying to do whatever interests me and lately I've gotten the case of wanderlust and wanted to get back into embedded programming specifically with robotics in mind.

# Gotta start somewhere

I originally had thought my entry was to get a Lego Mindstorms kit and begin working with that, but a friend had been interested in quadcopters both in autonomous flight and just as a hobby.  When I did some research and realized that there was some substantial work to bring autopilots to the average person without custom building a lot of avionics, the decision became obvious.

I picked up the 3DR Iris in April of 2014 as a way to get a RTF kit after spending a couple months researching different autopilots. I also picked up a Heli-Max 1SQ (trainer) as my flight trainer since I actually enjoy flying the copter in full manual flight (stabilize only).

# My short term goals

As I spent a few weeeks in my off hours digging through code and trying to get get my environment and SITL runnning I realized there were some great first objectives along both the hardware and software.

## Software

*  The codebase needs a lot testing infrastructure.  I mean the whole kitchen sink needs to be added.  I want to apply some retroactive TDD into the codebase with unit tests followed by the final regression "autotest" suite as the icing on the cake.  Ideally I want all the critical software loops running with mocked (but realistic) data to ensure control loops are always well-tested.  This also is needed when I want to add additional flight features that I have some testing framework in place.
*  The virtual machine is unusuable for software testing.  But it should be!
*  I want to develop or adapt a software algorithm to compensate for prop failure to make a quadcopter more tolerant to motor failure.  Some people have work done on this and I want to see if this software is opened to the public (or maybe develop my own).
*  OpenGL frame simulation.  I want to visualize what the frame is doing especially when developing some extreme flight scenarios.
*  Add computer vision during autopilot mission.  Right now if you plot a mission course... and you don't notice something is in the path, you could plow the copter right into a tree or object.  Adding some sort of obstacle avoidance during autopilot would be nice to have.

## Hardware

* I need navigation LEDS that can both tell me the direction the copter is facing and communicate to me (e.g. pulse different light patterns depending on situation).
* I need to elevate the GPS out of the Iris.
* I need a consistent 12v -> 5v conversion tap for additional avionics
* I want to add sonar for more precise controlled landing and terrain follow
