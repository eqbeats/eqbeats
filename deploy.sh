#!/bin/bash

cd /home/eqbeats/eqbeats
make || return 1
install -m775 eqbeats.fcgi launch.sh failsafe transcode.sh hitsd.sh ytmgr.sh /srv/eqbeats/

cd tools
find . -maxdepth 1 -type f -executable -exec install -m775 '{}' /srv/eqbeats/tools/ \;


cd ../static
install -m664 *.css *.txt *.swf *.png *.jpg *.xml playing.ico 502.html /srv/eqbeats/static
for f in *.js; do
    jsmin < $f > /srv/eqbeats/static/$f
done

install -m664 favicon-pub.ico /srv/eqbeats/static/favicon.ico

cd icons
install -m664 * /srv/eqbeats/static/icons

sudo -u eqbeats-pub bash -c "EQBEATS_DIR=/srv/eqbeats EQBEATS_COUNT=5 /srv/eqbeats/launch.sh restart"
