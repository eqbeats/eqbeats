#include "follower.h"
#include "db.h"
#include <text/text.h>

bool favHelper(const std::string &t, int ref, int id){
    DB::Result r = DB::query(
        "SELECT EXISTS (SELECT 1 FROM favorites "
        "WHERE favorites.type = '" + t + "' "
        "AND favorites.user_id = " + number(id) + " "
        "AND favorites.ref = " + number(ref) +
        ")");
    if(r.empty()) return false;
    return r[0][0] == "t";
}

std::vector<User> Follower::following() const{
    return User::select("FROM users, favorites "
        "WHERE favorites.ref = users.id AND favorites.type = 'artist' "
        "AND favorites.user_id = " + number(id()) + " "
        "ORDER BY users.name ASC");
}

std::vector<std::string> Follower::followers() const{
    DB::Result r = DB::query(
        "SELECT users.email FROM users, favorites "
        "WHERE favorites.user_id = users.id AND favorites.type = 'artist' "
        "AND favorites.ref = " + number(id()));
    std::vector<std::string> emails(r.size());
    for(unsigned i=0; i<r.size(); i++)
        emails[i] = r[i][0];
    return emails;
}

bool Follower::isFollowing(int uid) const{
    if(id()<=0) return false;
    return favHelper("artist", uid, id());
}

void Follower::follow(int uid){
    if(uid == id()) return;
    if(isFollowing(uid)) return;
    DB::query("INSERT INTO favorites (user_id, type, ref) "
        "VALUES (" + number(id()) + ", 'artist', " + number(uid) + ")");
}

void Follower::unfollow(int uid){
    DB::query("DELETE FROM favorites "
        "WHERE type = 'artist' AND ref = " + number(uid) + " AND user_id = " + number(id()));
    DB::query("DELETE FROM events WHERE type = 'follow' AND source_id = " + number(id()) + " AND target_id = " + number(uid));
}

int Follower::followersCount() const{
    DB::Result r = DB::query("SELECT COUNT(*) FROM favorites "
        "WHERE type = 'artist' AND ref = " + number(id()));
    return r.empty() ? -1 : number(r[0][0]);
}

bool Follower::isFavorite(int tid) const{
    if(_id<=0) return false;
    return favHelper("track", tid, _id);
}

bool Follower::addToFavorites(int tid){
    if(isFavorite(tid)) return false;
    DB::query("INSERT INTO favorites (user_id, type, ref) "
        "VALUES (" + number(_id) + ", 'track', " + number(tid) + ")");
    return true;
}

void Follower::removeFromFavorites(int tid){
    DB::query("DELETE FROM favorites "
        "WHERE type = 'track' AND ref = " + number(tid) + " AND user_id = " + number(_id));
    DB::query("DELETE FROM events WHERE type = 'favorite' AND source_id = " + number(id()) + " AND track_id = " + number(tid));
}
