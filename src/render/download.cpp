#include "download.h"
#include "http.h"
#include "html/page.h"
#include "../number.h"
#include "../user.h"
#include <string>

using namespace Render;
using namespace std;

void Http::downloadTrack(int tid, Track::Format f){
    Track t(tid);
    if(!t) return Html::notFound("Track");
    string ext = f == Track::Vorbis ? ".ogg" : ".mp3";
    string mime = f == Track::Vorbis ? "ogg" : "mpeg";
    Http::download("/downloads/tracks/"+number(tid)+ext, t.artist().name()+" - "+t.title()+ext, "audio/"+mime, true);
}

void Http::trackArt(int tid, Art::Size sz){
    Art art(tid);
    if(!art) return Html::notFound("Art");
    Track t(tid);
    if(!t) return Html::notFound("Track");
    Art::Format f = art.getFormat();
    string ext = f == Art::PNG ? ".png" : f == Art::JPEG ? ".jpg" : f == Art::GIF ? ".gif" : "";
    string mime = f == Art::PNG ? "image/png" : f == Art::JPEG ? "image/jpeg" : f == Art::GIF ? "image/gif" : "application/octet-stream";
    Http::download((string)"/downloads/art/"+(sz==Art::Medium?"medium/":sz==Art::Thumbnail?"thumb/":"")+number(tid), t.artist().name()+" - "+t.title()+ext, mime);
}
