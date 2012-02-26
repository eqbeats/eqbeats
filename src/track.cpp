#include <string.h>
#include <sstream>
#include "track.h"
#include "user.h"
#include "number.h"
#include "db.h"
#include "art.h"

using namespace std;

Track::Track(int id){
    _id = 0;
    _artistId = 0;
    if(id<=0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date FROM tracks CROSS JOIN users "
        "WHERE tracks.id = " + number(id) + " AND tracks.user_id = users.id");
    if(!r.empty()){
        _id = id;
        _title = r[0][0];
        _artistId = number(r[0][1]);
        _artist = r[0][2];
        _visible = r[0][3] == "t";
        _date = r[0][4];
    }
}

User Track::artist() const{
    return User(_artistId, _artist);
}

void Track::setTitle(const std::string &nTitle){
    if(_title == nTitle) return;
    DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(_id), nTitle);
    _title = nTitle;
}

std::string Track::url(int id){
    std::stringstream s;
    s << "/track/" << id;
    return s.str();
}

void Track::setVisible(bool v){
    DB::query("UPDATE tracks SET visible = '" + (std::string)(v?"t":"f") + "' WHERE id = " + number(_id));
    _visible = v;
}

std::string Track::getNotes() const{
    DB::Result r = DB::query("SELECT notes FROM tracks WHERE id = " + number(_id));
    return r.empty() ? std::string() : r[0][0];
}
void Track::setNotes(const std::string &nNotes){
    DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(_id), nNotes);
}

std::string Track::url(Format f) const{
    string format = f==Vorbis? "vorbis" : "mp3";
    return (std::string) "/track/" + number(_id) + "/" + format;
}

Track Track::create(int nArtistId, const std::string &nTitle){
    Track t;
    DB::Result r = DB::query(
        "INSERT INTO tracks (user_id, title, date) VALUES "
        "("+number(nArtistId)+", $1, 'now') "
        "RETURNING id, date", nTitle);
    if(r.empty()) return t;
    return Track(number(r[0][0]), nTitle, nArtistId, User(nArtistId).name(), false, r[0][1]);
}

void Track::remove(){
    DB::query("DELETE FROM track_categories WHERE track_id = "+ number(_id));
    DB::query("DELETE FROM contest_submissions WHERE track_id = " + number(_id));
    DB::query("DELETE FROM featured_tracks WHERE track_id = " + number(_id));
    DB::query("DELETE FROM votes WHERE track_id = " + number(_id));
    DB::query("DELETE FROM comments WHERE type = 'track' AND ref = " + number(_id));
    DB::query("DELETE FROM favorites WHERE type = 'track' AND ref = " + number(_id));
    DB::query("DELETE FROM tracks WHERE id = " + number(_id));
    _id = 0;
}

int Track::getHits() const{
    DB::Result r = DB::query("SELECT hits FROM tracks WHERE id = " + number(_id));
    return number(r[0][0]);
}
void Track::hit(){
    DB::query("UPDATE tracks SET hits = hits+1 WHERE id = " + number(_id));
}

std::vector<Track> Track::resultToVector(const DB::Result &r){
    std::vector<Track> tracks(r.size());
    for(unsigned i=0; i<r.size(); i++)
        tracks[i] = Track(number(r[i][0]), r[i][1], number(r[i][2]), r[i][3], r[i][4]=="t", r[i][5]);
    return tracks;
}

std::vector<Track> Track::select(const char *tables, const std::string cond, const char *order, bool all, int limit){
    std::string q = "SELECT tracks.id, tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date FROM tracks, users";
    if(tables) q += (std::string) "," + tables;
    q += " WHERE tracks.user_id = users.id";
    if(!all) q += " AND tracks.visible='t'";
    if(!cond.empty()) q += " AND " + cond;
    q += (std::string) " ORDER BY " + order;
    if(limit) q += " LIMIT " + number(limit);
    return resultToVector(DB::query(q));
}

std::vector<Track> listTracks(const char *order, int limit){
    return Track::select(0, "", order, false, limit);
}

std::vector<Track> Track::latest(int n){ return listTracks("date DESC", n); }
std::vector<Track> Track::random(int n){ return listTracks("random()", n); }
std::vector<Track> Track::popular(int n){ return listTracks("hits DESC", n); }

std::vector<Track> Track::featured(int n){
    return Track::select("featured_tracks", "featured_tracks.track_id = tracks.id", "featured_tracks.date DESC", false, n);
}

std::vector<Track> Track::favorites(int uid){
    return select("favorites", 
        "favorites.type = 'track'"
        " AND favorites.ref = tracks.id "
        " AND favorites.user_id = " + number(uid),
        "users.name ASC");
}

#define SEL (std::string) ("SELECT tracks.id, tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date " \
                            "FROM tracks, users WHERE tracks.user_id = users.id AND tracks.visible = 't' ")

std::vector<Track> Track::search(const std::string &q){
    if(q.empty()) return std::vector<Track>();
    std::vector<std::string> p;
    std::istringstream in(q);
    std::string buf;
    std::string sql;
    while(in){
        in >> buf;
        p.push_back("%"+buf+"%");
        sql += " AND (tracks.title ILIKE $" + number(p.size()) + " OR users.name ILIKE $" + number(p.size()) + ")";
    }
    return resultToVector(DB::query(SEL + sql, p));
}

std::vector<Track> Track::exactSearch(const std::string &qartist, const std::string &qtitle){
    if(qtitle.empty()||qartist.empty()) return std::vector<Track>();
    return resultToVector(DB::query(SEL + "AND users.name = $1 AND tracks.title = $2", qartist, qtitle));
}

int Track::favoritesCount() const{
    DB::Result r = DB::query("SELECT COUNT(*) FROM favorites "
        "WHERE type = 'track' AND ref = " + number(_id));
    return r.empty() ? -1 : number(r[0][0]);
}

void Track::bump(){
    if(_id<=0) return;
    DB::query("UPDATE tracks SET date = 'now' WHERE id = " + number(_id));
}
