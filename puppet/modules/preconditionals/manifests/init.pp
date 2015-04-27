class preconditionals {
#    file { '/etc/apt/sources.list':
#        source      => 'puppet:///modules/preconditionals/sources.list',
#        mode        =>  '666',
#    } ->

    exec { 'i386':
        command         =>   'dpkg --add-architecture i386'
    } ->
	exec { 'apt-update':
    	command => 'apt-get update',
	} ->
    package { 'build-essential':
        ensure          =>  'installed',
        provider        =>  'apt',
    }
}