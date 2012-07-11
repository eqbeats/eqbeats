#include "pages.h"
#include <account/session.h>
#include <core/cgi.h>
#include <social/event.h>
#include <social/follower.h>
#include <text/text.h>
#include <track/audio.h>
#include <track/extended.h>

void Pages::track(Document *doc){

    std::string sub;
    int tid = route("track", path, sub);

    if(!tid || sub != "") return;

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

    int hits = t.artist.self() ? t.getHits() : t.hit();
    if(hits) doc->dict()->SetValueAndShowSection("HIT_COUNT", number(hits), "HAS_HITS");

    Session::fill(doc->dict());
    EventList::track(t).fill(doc->dict(), "EVENTS");
    doc->dict()->ShowSection(Follower(Session::user().id).favorited(tid) ? "IS_FAVORITE" : "NOT_FAVORITE");

}
