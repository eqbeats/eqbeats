#include "pages.h"
#include "../audio.h"
#include "../extended.h"
#include <core/cgi.h>
#include <stat/stat.h>
#include <text/text.h>
#include <unistd.h>
#include <render/file.h>

void Pages::trackMisc(Document *doc){

    std::string sub;
    int tid = route("track", path, sub);
    if(!tid) return;

    if(sub == "embed"){
        Track t(tid);
        if(t){
            t.fill(doc->dict());
            t.player(doc->dict(), true);
        }
        doc->setTemplate("html/player-embed.tpl", "text/html", t ? 200 : 404);
        doc->dict()->ShowSection(t ? "FOUND" : "NOT_FOUND");
        return;
    }

    else if(sub == "json"){
        ExtendedTrack t(tid);
        if(!t) return;
        doc->setJson("json/track.tpl");
        t.fill(doc->dict());
    }

    // Download

    else if(sub == "original" || sub == "vorbis" || sub == "mp3"
            || sub == "aac" || sub == "opus"){
        Track t(tid);
        if(!t)
            return;
        else if(sub == "original")
            doc->download(Audio(&t).original(), true);
        else if(sub == "vorbis")
            doc->download(Audio(&t).vorbis(), true);
        else if(sub == "aac")
            doc->download(Audio(&t).aac(), true);
        else if(sub == "opus")
            doc->download(Audio(&t).opus(), true);
        else
            doc->download(Audio(&t).mp3(), true);
        if(cgi("stream").empty())
            Stat::push("trackDownload", t.artist.id, tid);
    }

    else if(sub == "played"){
        Track t(tid);
        if(t)
            Stat::push("trackPlay", t.artist.id, tid);
        doc->setJson("/dev/null");
    }

}
