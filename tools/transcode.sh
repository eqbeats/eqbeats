#!/bin/bash

if [[ "$#" -ne 2 ]]; then
    echo "Usage: $0 ID INPUT" >&2
    exit 1
fi

BASE="$EQBEATS_DIR/tracks/$1"
find "$EQBEATS_DIR/tracks" -name "$1.*" -delete

ffmpeg -loglevel quiet -y -i "$2" -acodec libvorbis -b:a 128k -vn "$BASE.ogg" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"

[ $? -ne 0 ] && exit 1

if [[ $(file -b --mime-type "$2") = "audio/mpeg" ]]
then
    mv "$2" "$BASE.mp3"
    ln -s "$1.mp3" "$BASE.orig.mp3"
else
    ffmpeg -loglevel quiet -y -i "$2" -acodec libmp3lame -q:a 0 -vn "$BASE.mp3" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"
    mv "$2" "$BASE.orig.${2##*.}"
fi

updatetags "$1"
