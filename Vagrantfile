# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|

    config.vm.define "trusty64" do |trusty64|
        trusty64.vm.box = "ubuntu-14.04-bit"
        trusty64.vm.box_url = "http://dps.edgesuite.net/vagrant/ubuntu-14.04-amd64-vbox.box"
        trusty64.vm.hostname = "apm-dev"
#        trusty64.vm.network "public_network"
        trusty64.vm.synced_folder(".", "/home/vagrant/ardupilot" )
        trusty64.vm.synced_folder("../PX4Firmware", "/home/vagrant/PX4Firmware" )
        trusty64.vm.synced_folder("../PX4NuttX", "/home/vagrant/PX4NuttX" )
        trusty64.vm.synced_folder("../../mavlink/mavlink", "/home/vagrant/mavlink" )

        trusty64.vm.provision "shell", inline: "cp /home/vagrant/ardupilot/puppet/Puppetfile /tmp"
        trusty64.vm.provision "shell", inline: "cp -R /home/vagrant/ardupilot/puppet/modules /tmp"
        trusty64.vm.provision "shell", inline: "cp -R /home/vagrant/ardupilot/puppet/manifests /tmp"

        # Puppet, install pre-reqs libraries
        trusty64.vm.provision "puppet" do |vb_provision_init|
            vb_provision_init.temp_dir = "/tmp"
            vb_provision_init.options = [ '--modulepath=/tmp/modules' ]
            vb_provision_init.manifests_path = [ "vm", "/tmp/manifests" ]
            vb_provision_init.manifest_file = "init.pp"
        end

		# Install librarian-puppet
        trusty64.vm.provision "shell", inline: "gem install librarian-puppet"
        trusty64.vm.provision "shell", inline: "cd /tmp && librarian-puppet install --verbose"

        # Puppet, finish the provisioning
        trusty64.vm.provision "puppet" do |vb_provision|
            vb_provision.temp_dir = "/tmp"
            vb_provision.options = [ '--modulepath=/tmp/modules' ]
            vb_provision.manifests_path = [ "vm", "/tmp/manifests" ]
            vb_provision.manifest_file = "apm_dev.pp"
        end

        # VBox Manage
        trusty64.vm.provider :virtualbox do |vb_config|
            # Allow symlinks, your machine should be administrator
            vb_config.customize ["setextradata", :id, "VBoxInternal2/SharedFoldersEnableSymlinksCreate/v-root", "1"]

            # Otherwise the compile will go into swap, making things slow
            vb_config.customize ["modifyvm", :id, "--memory", "2048"]
            vb_config.customize ["modifyvm", :id, "--vram", "256"]
            vb_config.customize ["modifyvm", :id, "--cpus", "4"]
            vb_config.customize ["modifyvm", :id, "--ioapic", "on"]
            vb_config.customize ["modifyvm", :id, "--cpuexecutioncap", "100"]
            vb_config.gui = true
    	end
    end
end


#
