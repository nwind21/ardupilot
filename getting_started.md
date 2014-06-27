---
title: Getting Started
layout: default
---

This is my little working folder with some mods for my work with Pixhawk.  I don't always guarantee that the other APM2 etc VRBrain, etc will
work since those platforms are not of interest to me.  I do plan to try and make sure I don't break anything that the Pixhawk drives like
ArduPlane or ArduRover.

## Getting the source

I like to group my git clones by the root, so diydrones, tridge, mavlink etc would be the top level folder.
So here, since we're using my fork, it's in nwind21 (I have another clone from diydrons in .\diydrones\ardupilot)

```
git clone git://github.com/nwind21/ardupilot.git C:\<your git working root>\nwind21\ardupilot
git clone git://github.com/diydrones/PX4Firmware.git C:\<your git working root>\diydrones\PX4Firmware
git clone git://github.com/diydrones/PX4NuttX.git C:\<your git working root>\diydrones\PX4NuttX
git clone git://github.com/tridge/jsbsim.git C:\<your git working root>\tridge\jsbsim
```

## Development using VirtualBox

ardupilot has a standardized Linux virtual machine (VM) setup script
that uses the free VirtualBox virtualization software.  You can use it
to create a standard, reproducible development environment in just a
few minutes in Linux, OS X, or Windows.

 1. [Download VirtualBox](https://www.virtualbox.org/wiki/Downloads)
 for your Mac, Windows or Linux machine.

 2. [Install vagrant](http://docs.vagrantup.com/v2/installation/).
 
 3. <strong>Enable Virtualization in BIOS</strong>  This vagrantfile will instruct Vbox provisioner
 to enable multi-core of 4 with a CPU cap of 80%. If you do not, the SITL will be extremely slow.  If you don't
 enable Virtualization options, the box will fail to boot.
 
 4. <strong>Edit the vagrantfile</strong> shared_folders to map to the three required projects: ardupilot, PX4Nuttx, PX4Firmware, and jsbsim.

 5. <strong>Start the Vagrant</strong> In the `ardupilot` directory, run `vagrant up` from the command
 line.  This will create a new Ubuntu Linux VM.  This box is provisioned via puppet.
 any updates to the provisioning can be applied by running `vagrant provision`.
 The puppet files mimic the setup in "ardupilot/Tools/scripts/install-prereqs-ubuntu.sh -y"` as well
 as the requirements for SITL.  You do NOT need to run the script.  Puppet handles the provisioning.
 
 6. <strong>Wait for Puppet to complete provisioning</strong> Once the provisioning is complete (it takes a while (30min or so) due to the large UI install packages)
 
 7. <strong>Reboot</strong> to get a nice UI] run `vagrant reload` and upon reboot, the UI will appear. I recommend logging in using GNOME 2d (no effects) and installing the 
 GuestBoxAdditions from the Guest machine.

 8. <strong>Login</strong> Login password is "vagrant" as well as sudo password.
 
 9. <strong>Fixing the guest additions defect in VBox 4.3.10</strong> VirtualBox 4.3.10 has a a defect in the VBoxAdditions.  You will need to run the script "sudo vbox_patch.sh" located in
 home directory then reload the machine with `vagrant reload` otherwise vagrant will fail to mount the shared folders AFTER
 you've installed the VBoxAdditions.

Once you've followed the instructions above, here's how you would
build ArduCopter for PX4 in the development environment:

```
# cd ardupilot/ArduCopter
# make configure
```

Back at the terminal:

```
# make px4
# make px4-upload  # (optional)
```

I personally like to run the Software In Test Loop to make sure your build and test environment is sane...

## Software In Test Loop

If I did everything properly in the VM, you should be able to do:

```
# cd ardupilot/Tools/autotest
# autotest_n21.py --mode quadcopter
```

And that should build the quadcopter SITL and run the simulation.  If you're making a code change and just wanna re-build incrementally:

```
# autotest_n21.py --mode build-quadcopter --incremental
```

or just re-fly

```
# autotest_n21.py --mode fly-quadcopter
```
