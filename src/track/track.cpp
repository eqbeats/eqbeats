#include "track.h"
#include <account/user.h>
#include <core/db.h>
#include <misc/number.h>
#include <session/session.h>
#include <text/text.h>

Track::Track(int tid){

    id = 0;
    if(tid <= 0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date FROM tracks, users "
        "WHERE tracks.id = " + number(tid) + " AND tracks.user_id = users.id");

    if(!r.empty()){
        id = tid;
        title = r[0][0];
        artist = User(number(r[0][1]), r[0][2]);
        visible = r[0][3] == "t";
        date = r[0][4];
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
    d->SetValue("DAY", day(date));
    d->ShowSection(artist == Session::user() ? "IS_OWNER" : "IS_NOT_OWNER");
    artist.fill(d);
}
