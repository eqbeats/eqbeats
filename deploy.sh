#!/bin/bash

cd /home/eqbeats/eqbeats
make || return 1
install -m775 eqbeats.fcgi failsafe hitsd.sh /srv/eqbeats/

cd static
install -m664 *.js *.css *.txt *.swf *.png 502.html /srv/eqbeats/static

install -m664 favicon-pub.ico /srv/eqbeats/static/favicon.ico

sudo -u eqbeats-pub bash -c "EQBEATS_DIR=/srv/eqbeats /srv/eqbeats/launch.sh restart"
