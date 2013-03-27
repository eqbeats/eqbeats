#!/bin/bash

cd $EQBEATS_DIR

function start(){
  ./rc/eqbeats start || exit 1
  ./rc/hitsd   start || exit 1
  ./rc/ytmgr   start || exit 1
  ./rc/udpstat start || exit 1
}

function stop(){
  ./rc/udpstat stop || exit 1
  ./rc/ytmgr   stop || exit 1
  ./rc/hitsd   stop || exit 1
  ./rc/eqbeats stop || exit 1
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
    sleep 0.5
    start
    ;;

  *)
    echo "usage: $0 {start|stop|restart}"
esac

exit 0

# vim: sw=2 ts=2
