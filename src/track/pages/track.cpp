#include "pages.h"
#include "../audio.h"
#include "../extended.h"
#include <core/cgi.h>
#include <text/text.h>

void Pages::track(Document *doc){
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

    // Access

    if(sub == ""){
        ExtendedTrack t(tid);
        if(!t) return;
        doc->setHtml("html/track.tpl", t.title);
        doc->rootDict()->SetValueAndShowSection("TID", number(t.id), "HAS_OEMBED");
        t.fill(doc->dict());
        t.player(doc->dict(), true);
        Audio(&t).fill(doc->dict());
        Dict *embed = doc->dict()->AddIncludeDictionary("EMBED_CODE");
        embed->SetFilename("embed-code.tpl");
        embed->SetIntValue("WIDTH", 150);
        t.Track::fill(embed);
        Dict *uploader = doc->dict()->AddIncludeDictionary("UPLOADER");
        uploader->SetFilename("html/uploader.tpl");
        uploader->SetValue("ACTION", t.url() + "/upload");
    }

    else if(sub == "json"){
        ExtendedTrack t(tid);
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
