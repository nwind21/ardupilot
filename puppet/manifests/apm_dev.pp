Exec {path => [ "/usr/local/bin/", "/bin/", "/usr/bin/", "usr/sbin/", "/sbin/", "/usr/sbin/" ]}


include development::ui
include development::sitl
include development::px4
include development::desktop
class { 'development::base':
	user => "vagrant",
}
class { 'profile::base':
	user => "vagrant",
}
