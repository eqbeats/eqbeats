#!/bin/bash

PROGNAME='eqbeats fcgi server'
BINARY='eqbeats.fcgi'
SOCKET='eqbeats.sock'
SERVCOUNT=${EQBEATS_COUNT:=1}

cd $EQBEATS_DIR

function start()
{
    SERVRUNNING=$(ls /proc | egrep -c "^($(cat pid 2> /dev/null | tr "\n" "|"))$")
    if [[ $SERVRUNNING -ge $SERVCOUNT ]]; then
      echo "$PROGNAME : Already running"
    else
      export PATH="$PATH:$EQBEATS_DIR/tools"
      spawn-fcgi -F $SERVCOUNT -P pid -f ./$BINARY -s $SOCKET -M 0775 > /dev/null
      chgrp http eqbeats.sock
      echo "$PROGNAME : Started $SERVCOUNT."
    fi
}

function stop()
{
    if [[ ! -a pid ]]; then
      echo "$PROGNAME : Not running."
    else
      echo -n   "$PROGNAME : Stopping gracefully..."
      kill $(cat pid) > /dev/null 2>&1
      SERVRUNNING=1;
      while [[ $SERVRUNNING -gt 0 ]]; do
        sleep 0.1
        SERVRUNNING=$(ls /proc | egrep -c "^($(cat pid 2> /dev/null | tr "\n" "|"))$")
      done
      rm pid
      echo -e "\r$PROGNAME : Stopped.              "
    fi
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
