class profile::base ( $user = vagrant ) {

	# MAVPROXY PATHS
    file_line { 'autotest-path':
        line  => 'export PATH=$PATH:/home/vagrant/ardupilot/Tools/autotest',
        path  => "/home/${user}/.profile"
    }
    file_line { 'MAVProxy-path':
        line  => 'export PATH=$PATH:/home/vagrant/MAVProxy',
        path  => "/home/${user}/.profile"
    }
    file_line { 'MAVProxy-path-examples':
        line  => 'export PATH=$PATH:/home/vagrant/mavlink/pymavlink/examples',
        path  => "/home/${user}/.profile"
    }

	# CCACHE PATHS
    file_line { 'ccache-path':
        line  => 'export PATH=$PATH:/usr/lib/ccache',
        path  => "/home/${user}/.profile"
    }

    file_line { 'gcc-arm-path':
        line  => 'export PATH=$PATH:/home/vagrant/gcc-arm-none-eabi-4_8-2013q4/bin',
        path  => "/home/${user}/.profile"
    }
}
