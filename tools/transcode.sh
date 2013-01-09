#!/bin/bash

if [[ "$#" -ne 2 ]]; then
    echo "Usage: $0 ID INPUT" >&2
    exit 1
fi

BASE="$EQBEATS_DIR/tracks/$1"
find "$EQBEATS_DIR/tracks" -name "$1.*" -delete

TMP="/tmp/${UID}-${1}.wav"

ffmpeg -loglevel error -probesize 10000000 -i "$2" -y "$TMP" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"

[ $? -ne 0 ] && rm -f "$TMP" && exit 1

ffmpeg -loglevel error -i "$TMP" -acodec libvorbis -q:a 4 "$BASE.ogg" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"
ffmpeg -loglevel error -i "$TMP" -acodec libfaac -q:a 90 "$BASE.aac" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"
ffmpeg -loglevel error -i "$TMP" -acodec libopus -b:a 128k "$BASE.opus" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"


if [[ $(file -b --mime-type "$2") = "audio/mpeg" ]]
then
    mv "$2" "$BASE.mp3"
    ln -s "$1.mp3" "$BASE.orig.mp3"
else
    ffmpeg -loglevel error -probesize 10000000 -y -i "$TMP" -acodec libmp3lame -q:a 0 -vn "$BASE.mp3" 2>&1 >> "$EQBEATS_DIR/ffmpeg.log"
    mv "$2" "$BASE.orig.${2##*.}"
fi

updatetags "$1"
