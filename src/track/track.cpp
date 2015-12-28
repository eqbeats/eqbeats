#include "track.h"
#include "art.h"
#include <core/db.h>
#include <text/text.h>

#include <log/log.h>
#include <playlist/playlist.h>
#include <track/audio.h>

Track::Track(int tid){

    id = 0;
    if(tid <= 0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date, extract(epoch from tracks.date) FROM tracks, users "
        "WHERE tracks.id = " + number(tid) + " AND tracks.user_id = users.id");

    if(!r.empty()){
        id = tid;
        title = r[0][0];
        artist = User(number(r[0][1]), r[0][2]);
        visible = r[0][3] == "t";
        date = r[0][4],
        timestamp = r[0][5];
    }

}

std::string Track::url() const{
    return (std::string) "/track/" + number(id);
}

void Track::fill(Dict *d) const{
    d->SetIntValue("TID", id);
    d->SetValue("TITLE", title);
    d->ShowSection(visible ? "IS_VISIBLE" : "IS_HIDDEN");
    d->SetValue("DATE", date);
    d->SetValue("TIMESTAMP", timestamp);
    d->SetValue("DAY", formatTime(date, "%B %-d, %Y"));
    artist.fill(d);
    d->ShowSection(Art(id) ? "HAS_ART" : "NO_ART");
}

Dict* Track::player(Dict *d, bool fallback) const{
    Dict *p = d->AddIncludeDictionary("PLAYER");
    p->SetFilename("html/player.tpl");
    fill(p);
    p->SetIntValue("LIST", id);
    if(fallback) p->ShowSection("FALLBACK");
    return p;
}

