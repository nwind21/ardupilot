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
#    common::netinstall{ 'install_gmock':
#        url                  =>   'https://googlemock.googlecode.com/files/gmock-1.7.0.zip',
#        extracted_dir        =>   'gmock-1.7.0',
#        destination_dir      =>   "/home/${user}",
#        extract_command      =>   'unzip',
#        postextract_command  =>   'g',
#   } ->
#    exec { 'build_gtest':
#        cwd             =>  '/usr/src/gmock/gtest',
#        command         =>  'mkdir build && mkdir /usr/include/gtest && cmake -E chdir build cmake .. && cmake --build build && cp build/libgtest* /usr/local/lib && cp -R include/gtest/* /usr/include/gtest',
#        path            =>  '/usr/bin:/usr/sbin:/sbin:/bin',
#    }


#    package { 'arduino-core':
#        ensure          =>  'installed',
#        provider        =>  'apt',
#    }

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
