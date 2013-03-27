#!/bin/bash

PROGNAME='eqbeats fcgi server'
BINARY='eqbeats.fcgi'
SOCKET='eqbeats.sock'
SERVCOUNT=${EQBEATS_COUNT:=1}

cd $EQBEATS_DIR

function start()
{
    SERVRUNNING=$(pgrep -c -u $USER "^eqbeats.fcgi$")
    if [[ $SERVRUNNING -ge $SERVCOUNT ]]; then
      echo "$PROGNAME : Already running"
    else
      export PATH="$PATH:$EQBEATS_DIR/tools"
      spawn-fcgi -F $SERVCOUNT -f ./$BINARY -s $SOCKET -M 0775 > /dev/null
      chgrp http eqbeats.sock
      echo "$PROGNAME : Started $SERVCOUNT."
    fi
}

function stop()
{
    echo -n   "$PROGNAME : Stopping gracefully..."
    pkill -u $USER "^eqbeats.fcgi$" > /dev/null 2>&1
    SERVRUNNING=1;
    while [[ $SERVRUNNING -gt 0 ]]; do
      sleep 0.1
      SERVRUNNING=$(pgrep -c -u $USER "^eqbeats.fcgi$")
    done
    echo -e "\r$PROGNAME : Stopped.              "
}

case "$1" in
  start)
    start
    ;;
  stop)
    stop
    ;;
  restart)
    stop
    sleep 0.5 # wait for the socket to close properly
    start
    ;;

  *)
    echo "usage: $0 {start|stop|restart}"
esac

exit 0

# vim: sw=2 ts=2
