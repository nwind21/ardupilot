---
title: Puppet Librarian
layout: posts
categories: software
---

# Using Puppet Librarian

I had some feedback on the inclusion of stdlib which is a puppet forge module.  The feedback was to not include it as part of the project's repository since it is a 3rd party module. To remove the source from the repository it requires a little change to how we approach the Puppet provisioning.

Doing some research I came across this developer's approach and modified the approach slightly

[James Carr, Effective Puppet Module Management In Vagrant](http://blog.james-carr.org/2014/05/05/effective-puppet-module-management-in-vagrant/)

## Problems

<strong>Problem #1:</strong> is that `librarian-puppet` v1.1.x requires Ruby 1.9.3 and higher but Ubuntu 12.04 supports 1.8.7. I decided to keep it simple and stay with the `librarian-puppet` v1.0.x

<strong>Problem #2:</strong> was that I use a `url_parse.rb` command that was pull requested into `stdlib`, but for whatever reason, never merged.  I had to move this out to my "common" module since I needed this function.

<strong>Problem #3:</strong> is that my puppet files are on the host.  I have to modify the Vagrantfile to move all the puppet files to the guest then execute the provisioning from there.  This actually works out great because now the host doesn't need to install Ruby just to get `librarian-puppet`.  All the dependencies for puppet to run are hosted on the guest VM.

<strong>Problem #4:</strong> `librarian-puppet` requires git.  Vagrantfiles are executed in order so I have to actually run puppet once to just install git, prior to copying and installing the `librarian-puppet`, then I can run the full provisioning.

## Changes to the Vagrantfile

```
[...]

config.vm.synced_folder(".", "/home/vagrant/ardupilot" )
config[...]

[...]

# Puppet
config.vm.provision "puppet" do |vb_provision_init|
    vb_provision_init.temp_dir = "/tmp"
    vb_provision_init.options = [ '--modulepath=/tmp/modules' ]
    vb_provision_init.manifests_path = [ "vm", "/tmp/manifests" ]
    vb_provision_init.manifest_file = "init.pp"
end

[...]

config.vm.provision "shell", inline: "gem install librarian-puppet -v 1.0.3"
config.vm.provision "shell", inline: "cp /home/vagrant/ardupilot/puppet/Puppetfile /tmp"
config.vm.provision "shell", inline: "cp -R /home/vagrant/ardupilot/puppet/modules /tmp"
config.vm.provision "shell", inline: "cp -R /home/vagrant/ardupilot/puppet/manifests /tmp"
config.vm.provision "shell", inline: "cd /tmp && `librarian-puppet` install --verbose"

# Puppet
config.vm.provision "puppet" do |vb_provision|
    vb_provision.temp_dir = "/tmp"
    vb_provision.options = [ '--modulepath=/tmp/modules' ]
    vb_provision.manifests_path = [ "vm", "/tmp/manifests" ]
    vb_provision.manifest_file = "apm_dev.pp"
end
```

# Brief Explanation

So in sequence:

1. Run puppet once to execute `init.pp` which has the git install
2. Call the shell provisioner to install the gem for `librarian-puppet`
3. Copy all the puppet files to /tmp.  `librarian-puppet` looks for a specific file called `Puppetfile` which has his commands for dependencies.  In this case, just `stdlib` is a dependency.
4. Tell `librarian-puppet` to install the dependencies
5. Then run the final provisioning.
