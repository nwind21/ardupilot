class profile::vagrant {

	# MAVPROXY PATHS
    file_line { 'autotest-path':
        line  => 'export PATH=$PATH:/home/vagrant/ardupilot/Tools/autotest',
        path  => '/home/vagrant/.profile'
    }
    file_line { 'MAVProxy-path':
        line  => 'export PATH=$PATH:/home/vagrant/MAVProxy',
        path  => '/home/vagrant/.profile'
    }
    file_line { 'MAVProxy-path-examples':
        line  => 'export PATH=$PATH:/home/vagrant/mavlink/pymavlink/examples',
        path  => '/home/vagrant/.profile'
    }
	
	# CCACHE PATHS
    file_line { 'ccache-path':
        line  => 'export PATH=/usr/lib/ccache:$PATH',
        path  => '/home/vagrant/.profile'
    }
}
