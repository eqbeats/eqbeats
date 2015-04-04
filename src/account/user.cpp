#include "session.h"
#include <core/db.h>
#include <text/text.h>

#include <track/list.h>
#include <log/log.h>

User::User(int uid){
    id = 0;
    if(uid<=0) return;
    DB::Result r = DB::query("SELECT name FROM users WHERE id = " + number(uid));
    if(!r.empty()){
        id = uid;
        name = r[0][0];
    }
}

User::User(const std::string &email){
    id = 0;
    DB::Result r = DB::query("SELECT id, name FROM users WHERE email = $1", email);
    if(!r.empty()){
        id = number(r[0][0]);
        name = r[0][1];
    }
}

bool User::self() const{
    return Session::user() == *this;
}

std::string User::url() const{
    return "/user/" + number(id);
}

void User::fill(Dict *d) const{
    if(!id) return;
    d->SetIntValue("UID", id);
    d->SetValue("USERNAME", name);
    d->ShowSection(self() ? "IS_SELF" : "NOT_SELF");
}

void User::deleteAccount(){
    log("Deleting account: " + name + " (" + number(id) + ")");

    TrackList tracks = Tracks::byUser(id, true);
    for (TrackList::iterator i = tracks.begin(); i != tracks.end(); i++)
        i->deleteTrack();

    // ew ew gross
    DB::query("DELETE FROM sessions WHERE user_id = " + number(id));
    DB::query("DELETE FROM comments WHERE author_id = " + number(id));
    DB::query("DELETE FROM comments WHERE type = 'user' AND ref = " + number(id));
    DB::query("DELETE FROM favorites WHERE user_id = " + number(id));
    DB::query("DELETE FROM favorites WHERE favorite_type = 'artist' AND ref = " + number(id));
    DB::query("DELETE FROM resets WHERE user_id = " + number(id));
    DB::query("DELETE FROM events WHERE source_id = " + number(id) + " OR target_id = " + number(id));
    DB::query("DELETE FROM playlists WHERE user_id = " + number(id));
    DB::query("DELETE FROM user_features WHERE user_id = " + number(id));
    DB::query("DELETE FROM users WHERE id = " + number(id));
}
