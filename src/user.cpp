#include <sstream>
#include "user.h"
#include "utils.h"

User::User(int nId){
    _id = 0;
    if(nId<=0) return;

    DB::Result r = DB::query("SELECT name FROM Users WHERE id=" + number(nId));
    if(!r.empty()){
        _id = nId;
        _name = r[0][0];
    }
} 

std::string User::url(int id){
    std::stringstream s;
    s << "/user/" << id;
    return s.str();
}

std::vector<User> User::listHelper(bool artists, unsigned int n, unsigned int begin){
    return resultToVector(DB::query(
        "SELECT id, name FROM users " +
        (std::string) (artists? "WHERE EXISTS ( SELECT 1 FROM tracks WHERE user_id = users.id AND visible = 't' ) " : "") +
        "ORDER BY lower(name) ASC LIMIT "+number(n)+" OFFSET "+number(begin)));
}

std::vector<User> User::resultToVector(const DB::Result &r){
    std::vector<User> users(r.size());
    for(unsigned i=0; i<r.size(); i++)
        users[i] = User(number(r[i][0]), r[i][1]);
    return users; 
}

std::vector<User> User::list(unsigned int n, unsigned int begin){
    return listHelper(false, n, begin);
}

std::vector<User> User::listArtists(unsigned int n, unsigned int begin){
    return listHelper(true, n, begin);
}

std::vector<User> User::search(const std::string &q){
    if(q.empty()) return std::vector<User>();
    return resultToVector(DB::query(
        "SELECT id, name FROM users WHERE name ILIKE $1 ORDER BY registration DESC", "%"+q+"%"));
}

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

bool User::isFavorite(int tid) const{
    if(_id<=0) return false;
    return favHelper("track", tid, _id);
}

void User::addToFavorites(int tid){
    if(isFavorite(tid)) return;
    DB::query("INSERT INTO favorites (user_id, type, ref) "
        "VALUES (" + number(_id) + ", 'track', " + number(tid) + ")");
}

void User::removeFromFavorites(int tid){
    DB::query("DELETE FROM favorites "
        "WHERE type = 'track' AND ref = " + number(tid) + " AND user_id = " + number(_id));
}

std::vector<User> User::following() const{
    return resultToVector(DB::query(
        "SELECT users.id, users.name FROM users, favorites "
        "WHERE favorites.ref = users.id AND favorites.type = 'artist' "
        "AND favorites.user_id = " + number(_id) + " "
        "ORDER BY users.name ASC"));
}

std::vector<std::string> User::followers() const{
    DB::Result r = DB::query(
        "SELECT users.email FROM users, favorites "
        "WHERE favorites.user_id = users.id AND favorites.type = 'artist' "
        "AND favorites.ref = " + number(_id));
    std::vector<std::string> emails(r.size());
    for(unsigned i=0; i<r.size(); i++)
        emails[i] = r[i][0];
    return emails;
}

bool User::isFollowing(int uid) const{
    if(_id<=0) return false;
    return favHelper("artist", uid, _id);
}

void User::follow(int uid){
    if(isFollowing(uid)) return;
    DB::query("INSERT INTO favorites (user_id, type, ref) "
        "VALUES (" + number(_id) + ", 'artist', " + number(uid) + ")");
}

void User::unfollow(int uid){
    DB::query("DELETE FROM favorites "
        "WHERE type = 'artist' AND ref = " + number(uid) + " AND user_id = " + number(_id));
}

int User::followersCount() const{
    DB::Result r = DB::query("SELECT COUNT(*) FROM favorites "
        "WHERE type = 'artist' AND ref = " + number(_id));
    return r.empty() ? -1 : number(r[0][0]);
}
