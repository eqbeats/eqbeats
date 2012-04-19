#!/bin/bash

cd "$EQBEATS_DIR"
SOCKET="/tmp/ytmgr-$UID.sock"

case "$1" in
  start)
    if [[ -f $SOCKET ]]; then 
        echo "ytmgr : Already running."
        exit 0
    fi
    python tools/ytmgr.py &
    echo "ytmgr : Started."
    ;;
  stop)
    echo "exit" | nc.openbsd -U "$SOCKET"
    sleep 0.5
    rm -f $SOCKET
    echo "ytmgr : Stopped."
    ;;
  restart)
    $0 stop
    $0 start
    ;;
  *)
    echo "Usage : $0 (start|stop|restart)"
esac
