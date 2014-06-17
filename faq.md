---
layout: default
---

# FAQ

Here's some questions and answers which may be of use.  The first section is a jump table to the actual questions.

This list should be fairly short.

<strong>General</strong>

* [Do I sync with DIYDrones/Master?](#q1)
* [Github Link?](#q2)
* [Development Environment](#q3)

<strong>Troubleshooting</strong>

* [Vagrant failed to bring up](#q4)

# General Questions

<a name="q1"></a>
##Do I sync with DIYDrones/Master?

Yes. This project is regularly synced with the master branch but it really depends on my schedule.

<a name="q2"></a>
##Where is your source?

You can find my fork here.

```
git clone git://github.com/nwind21/ardupilot.git C:\<your git working root>\nwind21\ardupilot
```

<a name="q3"></a>
##What is your development environment?

I use a Vagrant with Puppet provisioned 12.04 Precise 32 on a Lenovo Yoga i7 laptop with VTX enabled for multi-core support.

For editing I use: Notepad++, Eclipse

For documentation and this site, I use Jekyll.

#Troubleshooting

##Vagrant failed to bring up.
This can be a few things:
* Did you enable VTX in your BIOS?
* Do you have the right number of cores (edit the Vagrantfile)
* Do you have a network connection?  Puppet needs to get from ubuntu mirrors
