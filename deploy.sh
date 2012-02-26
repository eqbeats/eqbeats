#!/bin/bash

cd /home/eqbeats/eqbeats
make || return 1
install -m775 eqbeats.fcgi failsafe /srv/eqbeats/

install -m775 tools/nowplaying.{json,sh} /srv/eqbeats/tools

cd static
for f in *.js *.css *.txt *.swf *.png
do install -m664 $f /srv/eqbeats/static
done

install -m664 favicon-pub.ico /srv/eqbeats/static/favicon.ico

sudo -u eqbeats-pub bash -c "EQBEATS_DIR=/srv/eqbeats /srv/eqbeats/launch.sh restart"
