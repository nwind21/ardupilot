class ci::java {
	require preconditionals
	
    package { 'default-jre':
        ensure          =>  'installed',
        provider        =>  'apt',
    }
}