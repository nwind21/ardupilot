class development::ui {
    require preconditionals

    package { 'ubuntu-desktop':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'gnome-panel':
        ensure          =>  'installed',
        provider        =>  'apt',
        require         =>  Package [ 'ubuntu-desktop' ],
    }->
    package { 'gnome-tweak-tool':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    package { 'dkms':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

    file { '/home/vagrant/vbox_patch.sh':
        source      => 'puppet:///modules/development/vbox_patch.sh',
        mode        =>  '777',
    }
}
