Exec {path => [ "/usr/local/bin/", "/bin/", "/usr/bin/", "usr/sbin/", "/sbin/", "/usr/sbin/" ]}

# Build environment
include development::sitl
include development::px4

class { 'development::base':
	user => "ubuntu",
}
class { 'profile::base':
	user => "ubuntu",
}

# Continual Integration
class { 'ci::flask' }
class { 'ci::lighttpd' }
class { 'ci::java' }
