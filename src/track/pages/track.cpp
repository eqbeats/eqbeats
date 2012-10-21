#include "pages.h"
#include "../audio.h"
#include "../extended.h"
#include <core/cgi.h>

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

    else if(sub == "original"){
        Track t(tid);
        if(t) doc->download(Audio(&t).original());
    }
    else if(sub == "vorbis"){
        Track t(tid);
        if(t) doc->download(Audio(&t).vorbis());
    }
    else if(sub == "mp3"){
        Track t(tid);
        if(t) doc->download(Audio(&t).mp3());
    }

}
