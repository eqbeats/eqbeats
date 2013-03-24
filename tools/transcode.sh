#!/bin/bash

if [[ "$#" -ne 2 ]]; then
    echo "Usage: $0 ID INPUT" >&2
    exit 1
fi

BASE="$EQBEATS_DIR/tracks/$1"
find "$EQBEATS_DIR/tracks" -name "$1.*" -delete

ffmpeg -loglevel error -probesize 10000000 -i "$2" -acodec libvorbis -q:a 4 -vn "$BASE.ogg" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"

[ $? -ne 0 ] && rm -f "$BASE.ogg" && exit 1


ffmpeg -loglevel error -probesize 10000000 -i "$2" -strict -2 -acodec libfaac -q:a 90 -vn "$BASE.m4a" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"
ffmpeg -loglevel error -probesize 10000000 -i "$2" -acodec libopus -b:a 128k -vn "$BASE.opus" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"


if [[ $(file -b --mime-type "$2") = "audio/mpeg" ]]
then
    mv "$2" "$BASE.mp3"
    ln -s "$1.mp3" "$BASE.orig.mp3"
else
    ffmpeg -loglevel error -probesize 10000000 -i "$2" -acodec libmp3lame -q:a 0 -vn "$BASE.mp3" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"
    mv "$2" "$BASE.orig.${2##*.}"
fi

# if moodbar is available, get a .mood file
if command -v moodbar >/dev/null 2>&1 ; then
    moodbar "$BASE.mp3" -o "$BASE.mood" 2>&1 >> "$EQBEATS_DIR/moodbar.log"
fi

updatetags "$1"
