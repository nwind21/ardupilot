---
title: FAQ
layout: default
---

Here are some questions and answers which may be of use.  The first section is a jump table to the actual questions.

This list should be fairly short.

<strong>General</strong>

* [Do I sync with DIYDrones/Master?](#q1)
* [Github Link?](#q2)
* [Development Environment](#q3)

<strong>Troubleshooting</strong>

* [Vagrant failed to bring up](#q4)
* [Vagrant cannot mount shared folders](#q5)

<div class="hline"></div>

# General Questions

<a name="q1"></a>
###Do I sync with DIYDrones/Master?

Yes. This project is regularly synced with the master branch but it really depends on my schedule.

<a name="q2"></a>
###Where is your source?

You can find my fork here.

```
git clone git://github.com/nwind21/ardupilot.git C:\<your git working root>\nwind21\ardupilot
```

<a name="q3"></a>
###What is your development environment?

I use a Vagrant with Puppet provisioned 12.04 Precise 32 on a Lenovo Yoga i7 laptop with VTX enabled for multi-core support.

For editing I use: Notepad++, Eclipse

For documentation and this site, I use Jekyll.

<div class="hline"></div>
#Troubleshooting

<a name="q4"></a>
###Vagrant failed to bring up.
This can be a few things:

* Did you enable VTX in your BIOS?
* Do you have the right number of cores (edit the Vagrantfile)
* Do you have a network connection?  Puppet needs to get from ubuntu mirrors

<a name="q5"></a>
###Vagrant cannot mount shared folders
You probably have VirtualBox version 4.3.10 and installed the Guest Additions.

A. You can either upgrade to 4.3.12 (confirmed to be fixed) [Remounting VBox Additions ISO on VBox Upgrade]({{site.url}}/2014/05/01/VirtualBox-Guest-Additions/) _or_
B. Run the script `/home/vagrant/vbox_patch.sh` and restart your machine using `vagrant reload`