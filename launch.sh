#!/bin/bash

PROGNAME="eqbeats dev server"
BINARY="eqbeats.fcgi"
SERVCOUNT=1
PORT=9004
cd $EQBEATS_DIR

case "$1" in
  start)
    SERVRUNNING=$(ls /proc | egrep -c "^($(cat pid 2> /dev/null | tr "\n" "|"))$")
    if [[ $SERVRUNNING -ge $SERVCOUNT ]]; then
      echo "$PROGNAME : Already running"
    else
      $0 stop > /dev/null
      spawn-fcgi -F $SERVCOUNT -P pid -f ./$BINARY -a 127.0.0.1 -p $PORT > /dev/null
      echo "$PROGNAME : Started."
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
