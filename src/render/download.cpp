#include "download.h"
#include "http.h"
#include "html/page.h"
#include "../number.h"
#include "../user.h"
#include <string>
#include <unistd.h>

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
    bool orig = sz == Art::Full ? true : sz == Art::Thumbnail ? false : access(art.filepath(sz).c_str(), R_OK) != 0;
    Art::Format f = orig ? art.getFormat() : Art::PNG;
    string ext = f == Art::PNG ? ".png" : f == Art::JPEG ? ".jpg" : f == Art::GIF ? ".gif" : f == Art::TIFF ? ".tiff" : "";
    string mime = f == Art::PNG ? "image/png" : f == Art::JPEG ? "image/jpeg" : f == Art::GIF ? "image/gif" : f == Art::TIFF ? "image/tiff" : "application/octet-stream";
    Http::download((string)"/downloads/art/"+(orig?"":sz==Art::Medium?"medium/":sz==Art::Thumbnail?"thumb/":"")+number(tid)+(orig?"":".png"), t.artist().name()+" - "+t.title()+ext, mime);
}
