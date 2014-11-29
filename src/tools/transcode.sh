#!/bin/bash

if [[ "$#" -ne 2 ]]; then
    echo "Usage: $0 ID INPUT" >&2
    exit 1
fi

BASE="$EQBEATS_DIR/tracks/$1"
find "$EQBEATS_DIR/tracks" -name "$1.*" -delete

FFMPEG="ffmpeg -loglevel quiet -nostdin -probesize 10000000 -i \"$2\" -map 0:a"

$FFMPEG -acodec libvorbis -q:a 4 "$BASE.ogg"

[ $? -ne 0 ] && rm -f "$BASE.ogg" && exit 1

errors=0

$FFMPEG -acodec libfdk_aac -vbr 3 "$BASE.m4a" || ((errors++))
$FFMPEG -acodec libopus -b:a 128k "$BASE.opus" || ((errors++))

if [[ $(file -b --mime-type "$2") = "audio/mpeg" ]]
then
    mv "$2" "$BASE.mp3"
    ln -s "$1.mp3" "$BASE.orig.mp3"
else
    $FFMPEG -acodec libmp3lame -q:a 0 "$BASE.mp3" || ((errors++))
    mv "$2" "$BASE.orig.${2##*.}"
fi

if [ "x$(basename "$0")" == "x$0" ]; then
    tags="updatetags"
else
    tags="$(dirname "$0")/updatetags"
fi
"$tags" "$1" || ((errors++))

exit $errors
