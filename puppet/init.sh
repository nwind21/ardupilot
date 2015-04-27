#!/bin/bash
echo "info > Applying initial puppet provisioning."
puppet apply --modulepath ./modules manifests/init.pp
echo "info > Installing librarian-puppet."
gem install librarian-puppet
echo "info > Installing puppet libraries as define in Puppetfile."
librarian-puppet install --verbose
echo "info > Done."