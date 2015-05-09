class development::sitl {

    require preconditionals

    package { 'python-dev':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'python-wxgtk2.8':
        ensure          =>  'installed',
        provider        =>  'apt',
		require			=>  Package [ 'python-dev' ]
    }

    package { 'python-scipy':
        ensure          =>  'installed',
        provider        =>  'apt',
		require			=>  Package [ 'python-dev' ]
    }

    package { 'python-opencv':
        ensure          =>  'installed',
        provider        =>  'apt',
		require			=>  Package [ 'python-dev' ]
    }

    package { 'python-pip':
        ensure          =>  'installed',
        provider        =>  'apt',
		require			=>  Package [ 'python-dev' ]
    }

    exec { 'pip-pymavlink':
        command         =>   'pip install pymavlink',
        onlyif          =>   'test `pip freeze | grep -c "pymavlink"` -eq 0',
        require         =>    Package [ 'python-pip' ]
    }

    exec { 'pip-mavproxy':
        command         =>   'pip install MAVProxy',
        onlyif          =>   'test `pip freeze | grep -c "MAVProxy"` -eq 0',
        require         =>    Package [ 'python-pip' ]
    }

    common::netinstall{ 'zeromq-4':
        url                  =>   'http://download.zeromq.org/zeromq-4.0.4.tar.gz',
        extracted_dir        =>   'zeromq-4.0.4',
        destination_dir      =>   '/home/vagrant',
    } ->
    exec { 'pip-libzmq':
        command         =>   'pip install pyzmq',
        onlyif          =>   'test `pip freeze | grep -c "pyzmq"` -eq 0',
        require         =>   Package [ 'python-pip' ]
    }

}
