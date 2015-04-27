class ci::lighttpd {

    package { 'lighttpd':
        ensure          =>  'installed',
        provider        =>  'apt',
    }

}