---
layout: posts
title: "Floating Point Exception in AP_AHRS_NavEKF"
date: "2014-07-16"
---

# What is the AHRS

The AP_AHRS (Attitude Heading Reference System) module is responsible for providing all the attitude data to control decision clases (flying by mission, where am i?).

Two implementations exists:
1. The legacy DCM implementation
2. The Kalman Filter (EKF) which, by default, is disabled by default at the time of this writing. The filter variant was used in the AVC 2013 challenge.

## AP_AHRS and its derivations

AP_AHRS while specified as an interface, it is not. It is a mixture of implementation and pure virtuals. AP_AHRS_DCM is it's first level derivation and, by default, the main class.

```
AP_AHRS <- AP_AHRS_DCM <- AP_AHRS_NavEKF
```

AP_AHRS_NavEKF remains disabled unless the AHRS_EKF_USE flag is set in the parameters which will subsequently enabled the `_ekf_use` parameter variable. The entire implementation is also controlled by a compile time flag `AP_AHRS_NAVEKF_AVAILABLE`.

By default, the AP_AHRS_NavEKF object is always in use (assuming the compile time is enabled), except it's member functions will shunt to the AP_AHRS_DCM implementation if the function `using_EKF` return true. The `using_EKF` function evaluates:
* EKF healthly?
* _ekf_use enabled?
* EKF module started?

# Tracking the problem down

The floating point exception only occurs between 1-100 runs of the SITL and I had to narrow down the SITL to only Calibate and ARM. I also did an experiment to to run the sequence of calibrate, arm, and disarm in a single SITL run but that didn't trigger the FPE; probably suggests some data may be arriving from the simulation that is causing the FPE.

## Can I use GDB?

I was hoping that I could trigger the failure in one run of the SITL so I could attach GDB but the only way to reproduce the issue seems to be new runs of the SITL therefore making the GDB start-up each run laborious (probably not creative enough to figure out how to automate the GDB boot-up each time). With failure rates betwwn 1-100, I didn't want to sit through each pass triggering gdb. I resorted to some trace statements and some good 'ol fashion conquer and divde.

## Basic assumptions
* Once initialized, the CAL/ARM/DISARM sequence is stable. This means initial data during start-up is intermittently causing the implementation to trigger the FPE.
* The EKF has a delayed start-up time. The test sequence should not have enough time to actually trigger the EKF boot-up since the delay is greater than the test run time.
* FPE's are typically caused by division by zero (integers or floats) so I'll work with this assumption.

# The Chase

I put some small trace statements in the EKF code to tell me once he is initializing.  Note, in the assumptions, there is a 10sec delay before any EKF is initialized so in theory, I should never see the system initialize at all because of the quick test sequence.

In chronological order, I found the following problems:

1. `start_time_ms` doesn't appear to be initialized and every so often, will trigger an early start-up.  Interestingly enough, the FPE doesn't manifest, but the ARM motors will timeout consistently when the EKF is initialized early.
2. After fixing the `start_time_ms` to be initialized, I didn't see anymore failures.  But that just means the EKF is not starting up therefore not generating a FPE.
3. So I reverted the change and then that's where the next series of issue begin...

# The Ghost

The FPE on rebuild after removing my changes (e.g. back to the beginning) does not manifest anymore.  What I get now, instead is a failure to ARM motors.  Also, I noticed that the offsets being dumped look pretty clean (always zero); in the prior runs with the FPE I noticed that they would not always be 0.0.

After running this sequence hundreds of times, I've decided now is the time to begin writing a unit test harness, starting with the InertialSensor module which feeds into the EKF and DCM for Attitude control.

I have a feeling the defect is still there, but somehow the planets are aligning and the only real way to ensure there is no defect is to unit test the modules related.  This feeling about the bug still being present just manifesting differently is related to the occasional an motor arm failure every so often which doesn't seem to happen with the non EKF enabled runs.
