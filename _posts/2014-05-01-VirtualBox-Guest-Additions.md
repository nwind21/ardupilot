---
title: VirtualBox Won't Mount Guest Additions ISO
layout: posts
categories: software
---

If you've already installed VirtualBox Guest Additions, VBox leaves the original ISO mounted and you probably will get an error trying to install the new additions from your upgrade.

To fix this:

1. Shutdown the machine `vagrant halt`
2. Open VirtualBox.
3. Go to your "machine" settings.
4. Go to `storage`.
5. Unmount the VBoxAdditions.iso.
6. Restart your vagrant with `vagrant up` and reinstall Guest Additions.

Credit goes to the information I found posted here:

[Cannot mount vbox additions @ ubuntu forums](http://askubuntu.com/questions/321589/unable-to-mount-the-cd-dvd-image-on-the-machine-sandbox)
