---
layout: posts
title: "AP_InertialSensor_HIL Tests Complete"
date: "2014-08-03"
categories: testing
---

Probably not the most exciting of tests but it was a starting point and it's best to start testing the little things first; they almost always have a bigger impact on the code than we tend to think.

I tried to follow some my own guidelines in a prior post [GTest and GMock framework](http://nwind21.github.io/ardupilot/testing/2014/08/02/gtest-framework/)

#Timer Roll-Over

I noticed, while not critical, that the HIL code does not handle for timer roll-over (will occur every 71 minutes, I think). In simulation, it's possible that flights can run for a long period of time.  Real hardware with flight times of < 15min would get a hardware reset when the battery gets removed and therefore reset the timer.

## AP_Math delta_u32 function

I added a basic `delta_u32` function to `AP_Math` and _accompanying unit tests_ to handle timer roll-over.  Without a rollover check, if one were to occur, the gyro and accel health will intermittently flash to a "false" till the next refresh adjusts for the roll-over.

<strong>This probably impacts long flight simulations than real world.</strong>
