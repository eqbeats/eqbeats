#!/bin/bash

cd /home/eqbeats/eqbeats
make || return 1
mkdir -p /srv/eqbeats/{tools,static,templates/{html,json,oembed},rc}

install -m775 eqbeats.fcgi launch.sh failsafe /srv/eqbeats/

cd src/tools
find . -maxdepth 1 -type f -executable -exec install -m775 '{}' /srv/eqbeats/tools/ \;

cd ../../rc
find . -maxdepth 1 -type f -executable -exec install -m775 '{}' /srv/eqbeats/rc/ \;

cd ../templates
find . -type f -exec install -m664 '{}' /srv/eqbeats/templates/'{}' \;


cd ../static
install -m664 *.css *.txt *.png *.jpg *.woff *.ttf *.eot *.svg *.js 502.html /srv/eqbeats/static
#for f in *.js; do
#    jsmin < $f > /srv/eqbeats/static/$f
#done

install -m664 favicon-pub.ico /srv/eqbeats/static/favicon.ico

cd icons
install -m664 * /srv/eqbeats/static/icons

sudo -u eqbeats-pub -i bash -c "/srv/eqbeats/launch.sh restart"
