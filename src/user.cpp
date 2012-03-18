#include <sstream>
#include "user.h"
#include "number.h"
#include "track.h"
#include "db.h"

User::User(int nId){
    _id = 0;
    if(nId<=0) return;
    DB::Result r = DB::query("SELECT name FROM users WHERE id=" + number(nId));
    if(!r.empty()){
        _id = nId;
        _name = r[0][0];
    }
} 

User::User(const std::string &nName){
    _id = 0;
    if(nName.empty()) return;
    DB::Result r = DB::query("SELECT id FROM users WHERE name=$1", nName);
    if(!r.empty()){
        _id = number(r[0][0]);
        _name = nName;
    }
}

std::string User::url(int id){
    std::stringstream s;
    s << "/user/" << id;
    return s.str();
}

std::vector<User> User::select(const std::string &q, const std::string &param){
    std::string sql = "SELECT users.id, users.name " + q;
    DB::Result r = param.empty() ? DB::query(sql) : DB::query(sql, param);
    std::vector<User> users(r.size());
    for(unsigned i=0; i<r.size(); i++)
        users[i] = User(number(r[i][0]), r[i][1]);
    return users; 
}

std::vector<User> listHelper(bool artists, unsigned int n, unsigned int begin){
    return User::select("FROM users " +
        (std::string) (artists? "WHERE EXISTS ( SELECT 1 FROM tracks WHERE user_id = users.id AND visible = 't' ) " : "") +
        "ORDER BY lower(name) ASC LIMIT "+number(n)+" OFFSET "+number(begin));
}

std::vector<User> User::list(unsigned int n, unsigned int begin){
    return listHelper(false, n, begin);
}

std::vector<User> User::listArtists(unsigned int n, unsigned int begin){
    return listHelper(true, n, begin);
}

std::vector<User> User::search(const std::string &q){
    if(q.empty()) return std::vector<User>();
    return select("FROM users WHERE name ILIKE $1 ORDER BY registration DESC", "%"+q+"%");
}

User User::withEmail(const std::string &email){
    DB::Result r = DB::query("SELECT id, name FROM users WHERE email = $1", email);
    if(r.empty()) return User();
    return User(number(r[0][0]), r[0][1]);
}

std::vector<Track> User::tracks(bool all){
    if(_id<=0) return std::vector<Track>();
    return Track::select(0, "tracks.user_id = " + number(_id), "tracks.date DESC", all);
}
