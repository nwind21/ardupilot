---
title: Avionics Plate Part 1
layout: posts
---

# The idea...

The goal was to create an avionics plate to mount an [Arduino Uno](http://www.adafruit.com/products/50) and the [GPS Mast](https://store.3drobotics.com/products/gps-mast) to the Iris.  I considered a variety of options:

* Mount to the shell.  The curvature would probably be problematic to model and it would require some CAD work with a 3D printer.  No go on this idea.
* Mount to the underbelly.  The Iris already sits pretty low and the GPS mast needs to be elevated. Ditched this idea quick since I wanted both the Uno and the mast on one plate.
* Velcro to the shell.  Well, this would just look horrible.
* Make a 2D plate and drill holes with long stand-offs into the Iris' upper plate.  This was the winner.

The Uno would drive a series of [NeoPixels](http://www.adafruit.com/category/168) and connect to the Pixhawk via the i2c connector (I will need to splice one together).

# The process...

Using Inkscape and some callipers I wanted to design an avionics plate that would sit on top of the Iris shell.  At first I made the avionics plate huge (nearly covering the entire shell) then realized a lot of it was unnecessary.  After a series of prunes and dry runs/prints on my printer, I came to what I consider the bare minimum for the first revision.  It's really critical during this time that the design is printed on a printer, cut-out by hand, then checked against the craft and potential mounting locations.  I found several errors during this process. Also I used the hole spacing to create an acrylic cutting guide but later I plan to make a better cutting template.

The laser cutter will raster anything that is not 0.001" so I did my little branding on the upper left, the rest of the lines have to be 0.001" otherwise the laser cutter will not do a cut.

[FabLab Prepping for Laser Cutting](fablabsd.org/laser-cutter-files-preparation/)

The final design ended up like this (at least the first version).

<img src="https://dl.dropboxusercontent.com/u/4242148/Blog/IrisAvionicsMount.png" class="img-thumbnail"/>

The GPS mast is positioned in the back with the Uno mounted sideways.  I put in a bunch of cuts to reduce the weight and some cut-outs/vents for wiring to drop through the upper shell.

There are four mounting holes that will need to be drilled through the top shell and the top Iris plate (where the pixhawk sits).

# The cut...

I went to [FabLab](fablabsd.org/) and cut the design on a 1/8" clear acrylic.  Here is the final cut!

<img src="https://dl.dropboxusercontent.com/u/4242148/Blog/ClearPlate.jpg" class="img-thumbnail"/>

