#!/bin/bash

PROGNAME='eqbeats fcgi server'
BINARY='eqbeats.fcgi'
SOCKET='eqbeats.sock'
SERVCOUNT=${EQBEATS_COUNT:=1}

cd $EQBEATS_DIR

case "$1" in
  start)
    SERVRUNNING=$(ls /proc | egrep -c "^($(cat pid 2> /dev/null | tr "\n" "|"))$")
    if [[ $SERVRUNNING -ge $SERVCOUNT ]]; then
      echo "$PROGNAME : Already running"
    else
      spawn-fcgi -F $SERVCOUNT -P pid -f ./$BINARY -s $SOCKET -M 0775 > /dev/null
      chgrp http eqbeats.sock
      echo "$PROGNAME : Started $SERVCOUNT."
    fi
    ;;

  stop)
    if [[ ! -a pid ]]; then
      echo "$PROGNAME : Not running."
    else
      kill $(cat pid) > /dev/null 2>&1
      rm pid
      echo "$PROGNAME : Stopped."
    fi
    ;;

  restart)
    $0 stop
    sleep 0.5 # wait for the socket to close properly
    $0 start
    ;;

  *)
    echo "usage: $0 {start|stop|restart}"
esac

exit 0
