class ci::flask {
	require preconditionals

    exec { 'pip-flask':
        command         =>   'pip install flask',
        onlyif          =>   'test `pip freeze | grep -c "Flask"` -eq 0',
        require         =>    Package [ 'python-pip' ]
    }
}