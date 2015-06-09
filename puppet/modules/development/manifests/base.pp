class development::base ( $user = vagrant ) {
    require preconditionals

    package { 'gawk':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'make':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'cmake':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    # Need to build gtest after gmock so set an explicit dependency chain
    common::netinstall{ 'install_gmock':
        url                  =>   'https://googlemock.googlecode.com/files/gmock-1.7.0.zip',
        extracted_dir        =>   'gmock-1.7.0',
        destination_dir      =>   "/home/${user}",
        extract_command      =>   'unzip',
        postextract_command  =>   "/home/${user}/gmock-1.7.0/configure && make > /home/${user}/gmock-1.7.0/vagrant.install",
    } ->
    exec { 'build_gtest':
        cwd             =>  "/home/${user}/gmock-1.7.0/gtest",
        command         =>  'mkdir build && cmake -E chdir build cmake .. && cmake --build build',
        path            =>  '/usr/bin:/usr/sbin:/sbin:/bin',
        creates         =>  "/home/${user}/gmock-1.7.0/gtest/lib/.libs/libgtest.a"
    }  ->
    exec { 'copy-files':
      command         =>  "cp /home/${user}/gmock-1.7.0/gtest/lib/.libs/* /usr/lib && cp /home/${user}/gmock-1.7.0/lib/.libs/* /usr/lib",
      path            =>  '/usr/bin:/usr/sbin:/sbin:/bin',
    }

    package { 'curl':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'vim':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'terminator':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'ccache':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    exec { 'usermod-dialout':
        command         =>   "usermod -a -G dialout ${user}",
        onlyif          =>   'test `groups | grep -c "dialout"` -eq 0'
    }

    # Prevent conflicts with the firmware flash
    package { 'modemmanager':
        ensure          =>  'purged',
        provider        =>  'apt',
    }

    package { 'gpsbabel':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'imagemagick':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

}
