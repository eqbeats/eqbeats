#!/bin/bash

cd "$EQBEATS_DIR"
SOCKET="hitsd.sock"
DATA="hits.dat"

case "$1" in
  start)
    if [[ -f $SOCKET ]]; then
        rm $SOCKET
    fi
    tools/multiplex tools/clint "$SOCKET" 2> /dev/null &
    sleep 0.5
    echo "load $DATA" | nc -U "$SOCKET" > /dev/null
    echo "hitsd : Started."
    ;;
  stop)
    echo "save $DATA" | nc -U "$SOCKET" > /dev/null
    echo "exit" | nc -U "$SOCKET"
    echo "hitsd : Stopped."
    ;;
  restart)
    $0 stop
    $0 start
    ;;
  *)
    echo "Usage : $0 (start|stop|restart)"
esac
