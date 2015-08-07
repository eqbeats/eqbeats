#include "pages.h"
#include <core/cgi.h>
#include <core/db.h>
#include <log/log.h>
#include <misc/mail.h>
#include <playlist/playlist.h>
#include <social/event.h>
#include <social/follower.h>
#include <text/text.h>
#include <track/audio.h>
#include <track/art.h>
#include <track/extended.h>
#include <stat/stat.h>

static std::string filter(const std::string &str){
    std::string buf;
    for(std::string::const_iterator i = str.begin(); i!=str.end(); i++){
        if(*i != '\n' && *i != '\r') buf += *i;
    }
    return buf;
}

void Pages::track(Document *doc){

    std::string sub;
    int tid = route("track", path, sub);
    bool post = cgi.getEnvironment().getRequestMethod() == "POST";

    if(!tid) return;

    if(sub == ""){

        ExtendedTrack t(tid);
        if(!t) return;

        doc->setHtml("html/track.tpl", t.title);

        doc->rootDict()->SetValueAndShowSection("TID", number(t.id), "HAS_OEMBED");
        t.fill(doc->dict());
        t.player(doc->dict(), true);
        Audio(&t).fill(doc->dict());

        Dict *embed = doc->dict()->AddIncludeDictionary("EMBED_CODE");
        embed->SetFilename("html/embed-code.tpl");
        embed->SetIntValue("WIDTH", 150);
        t.Track::fill(embed);

        Dict *uploader = doc->dict()->AddIncludeDictionary("UPLOADER");
        uploader->SetFilename("html/uploader.tpl");
        uploader->SetValue("ACTION", t.url() + "/upload");

        EventList::track(t).fill(doc->dict(), "EVENTS");
    }
}
