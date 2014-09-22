---
layout: posts
title: "NeoPixels on Arduino Uno, Part 1"
date: "2014-09-13"
categories: software
---

# Project Location

Project is located here: [NeoPixels](https://github.com/nwind21/apm_neopixels)

# Class Diagram and Test

All test code will be use Google Mock and Google Test. All testing will be done on the host.

* VisualStudio 2013 used for the unit test harness and xcompile using [Visual Micro](http://www.visualmicro.com).
* Arduino uLibC++ port found [here](https://github.com/maniacbug/StandardCplusplus)

Pure interfaces classes will be written to abstract the Arduino code from the lighting control algorithm. Four interfaces and implementation have been created:

* II2C
* ILighting
* ISystem
* ITimer

The tests will focus enforcing the calling of the I2C and ILighting objects interfaces using mocked classes.  The actual implementation of the I2C and ILighting layer will be tested on the hardware with the goal of the interfaces  staying as close to the hardware as possible to minimize the amount of physical hardware testing required. The interfaces will also not expose implementation details, e.g. the interfaces should not make assumptions of the underlying hardware (in the event, it changes).

<img src="https://dl.dropboxusercontent.com/u/4242148/Blog/ArduinoClassDiagram.png" class="img-thumbnail"/>

# Lighting Control Object

The Lighting control object will accept both <strong>ITimer</strong> and <strong>ILighting</strong> object which will allow mock objects to be inserted during test.

```
LightingIrisImpl lights;
LightingControl::initialize( &lights,
                             &GLOBAL_TIMER );
```

The Lighting Control Object (LCO) was designed with the following requirements:

* <strong>Non-blocking</strong>. Because lights usually need delays to control the visibliity, the most commong implementation would use delays. This would cause the board to block and prevent other events from happening.
* <strong>Queueable</strong>. Because of the non-blocking nature, I need a queue so that when an event arrives to display a lighting pattern, they can be queued.  This way commands and lighting patterns are never lost.
* <strong>10ms resolution</strong>. The LCO will attach to the system timer event at 10ms interval that should be sufficient for lighting applications.

Events can be queued:

```
LightingControl::queueLightSequence( LightingControl::LightSequence_SystemCheck );
```

# Arduino Main Control Loop

The main loop in the Arduino will drive the system using a global event timer <strong>ITimer</strong>.

```
void loop()
{
    GLOBAL_TIMER.update();
}
```
