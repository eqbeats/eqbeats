#include "pages.h"
#include "../audio.h"
#include "../extended.h"
#include <core/cgi.h>
#include <stat/push.h>
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
            || sub == "aac" || sub == "opus" || sub == "mood"){
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
        else if(sub == "mood")
            doc->download(Audio(&t).mood(), true);
        else
            doc->download(Audio(&t).mp3(), true);
        if(cgi("stream").empty() && sub != "mood")
            pushStat("trackDownload", t.artist.id, tid);
    }

    else if(sub == "played"){
        Track t(tid);
        if(t)
            pushStat("trackPlay", t.artist.id, tid);
        doc->setJson("/dev/null");
    }

    else if(sub == "stats"){
        Track t(tid);
        std::string uri = "udpstat/tracks/" + number(tid) + ".json";
        std::string path = eqbeatsDir() + "/" + uri;
        if(t){
            if(access(path.c_str(), R_OK))
                doc->setJson("json/array.tpl");
            else
                doc->download(File(uri, number(tid) + ".json"));
        }
    }
}
