#!/bin/bash

EXEC="eqbeats.fcgi"
PROGNAME="eqbeats fastcgi server"

cd $EQBEATS_DIR

case "$1" in
  start)
    if [[ -a $EQBEATS_DIR/pid ]]; then
      echo "$PROGNAME : Already running, or leftover pid file? ($EQBEATS_DIR/pid)"
    else
      spawn-fcgi -F 1 -P $EQBEATS_DIR/pid -d $EQBEATS_DIR/ -f $EQBEATS_DIR/$EXEC -a 127.0.0.1 -p 9004 > /dev/null
      echo "$PROGNAME : Started."
    fi
    ;;

  stop)
    if [[ ! -a $EQBEATS_DIR/pid ]]; then
      echo "$PROGNAME : Not running."
    else
      kill $(cat $EQBEATS_DIR/pid)
      echo "$PROGNAME : Stopped."
      rm $EQBEATS_DIR/pid
    fi
    ;;

  restart)
    $0 stop
    $0 start
    ;;

  *)
    echo "usage: $0 {start|stop|restart}"
esac

exit 0
