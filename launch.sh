#!/bin/bash

pwd="/srv/eqbeats"
EXEC="eqbeats.fcgi"
PROGNAME="eqbeats fastcgi server"

cd $pwd

case "$1" in
  start)
    if [[ -a $pwd/pid ]]; then
      echo "$PROGNAME : Already running, or leftover pid file? ($pwd/pid)"
    else
      spawn-fcgi -F 1 -P $pwd/pid -d $pwd/ -f $pwd/$EXEC -a 127.0.0.1 -p 9003 > /dev/null
      echo "$PROGNAME : Started."
    fi
    ;;

  stop)
    if [[ ! -a $pwd/pid ]]; then
      echo "$PROGNAME : Not running."
    else
      kill $(cat $pwd/pid)
      echo "$PROGNAME : Stopped."
      rm $pwd/pid
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
