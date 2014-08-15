#include "account.h"
#include <core/db.h>
#include <misc/hash.h>
#include <text/text.h>
#include <youtube/youtube.h>

#include <track/list.h>

Account::Account(int uid){
    id = 0;
    if(uid<=0) return;
    DB::Result r = DB::query("SELECT name, email, about, notify, license FROM users WHERE id=" + number(uid));
    if(!r.empty()){
        id = uid;
        name = r[0][0];
        email = r[0][1];
        about = r[0][2];
        notify = r[0][3] == "t";
        license = r[0][4];
    }

    // Using LEFT JOIN / GROUP BY above would require grouping by all otherwise
    // selected attributes, we're probably better off this way.
    r = DB::query("SELECT COUNT(*) FROM favorites, tracks WHERE tracks.visible = TRUE and tracks.id = favorites.ref AND favorites.type = 'track' AND favorites.user_id = " + number(uid));
    num_favs = r.empty() ? 0 : number(r[0][0]);

    r = DB::query("SELECT COUNT(*) FROM favorites WHERE favorites.ref = " + number(uid) + " AND favorites.type = 'artist'");
    num_followers = r.empty() ? 0 : number(r[0][0]);

}

void Account::fill(Dict* d) const{
    User::fill(d);
    d->SetValue("EMAIL", email);
    d->SetValueAndShowSection("ABOUT", about, "HAS_ABOUT");
    d->SetValue("LICENSE", license);
    if(notify) d->ShowSection("NOTIFY");
    // Avatar
    d->SetValue("EMAIL_MD5", md5(email));
    Dict *a = d->AddIncludeDictionary("AVATAR");
    a->SetFilename("html/avatar.tpl");
    a->SetValue("EMAIL_MD5", md5(email));
    if(self()) a->ShowSection("IS_SELF");
    // YouTube
    d->ShowSection(Youtube(id) ? "YOUTUBE" : "NO_YOUTUBE");
    // Number of favorites
    if(num_favs > 0) {
        d->ShowSection("HAS_FAVS");
    }
    d->SetValue("NUM_FAVS", number(num_favs));
    if(num_followers > 0) {
        d->ShowSection("HAS_FOLLOWERS");
        if(num_followers > 1)
            d->ShowSection("FOLLOWERS_PLURAL");
    }
    d->SetValue("NUM_FOLLOWERS", number(num_followers));
}

void Account::deleteAccount(int id){
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
    DB::query("DELETE FROM youtube_access_tokens WHERE user_id = " + number(id));
    DB::query("DELETE FROM user_features WHERE user_id = " + number(id));
    DB::query("DELETE FROM users WHERE id = " + number(id));
}
