#!/bin/bash

cd /home/eqbeats/eqbeats
make || return 1
install -m775 eqbeats.fcgi /srv/eqbeats/
install -m664 static/[^d]* /srv/eqbeats/static/
sudo -u eqbeats-pub bash -c "EQBEATS_DIR=/srv/eqbeats /srv/eqbeats/launch.sh restart"
