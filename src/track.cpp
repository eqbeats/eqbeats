#include <string.h>
#include <sstream>
#include "track.h"
#include "user.h"
#include "utils.h"
#include "art.h"

using namespace std;

Track::Track(int id){
    _id = 0;
    _artistId = 0;
    if(id<=0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible FROM tracks CROSS JOIN users "
        "WHERE tracks.id = " + number(id) + " AND tracks.user_id = users.id");
    if(!r.empty()){
        _id = id;
        _title = r[0][0];
        _artistId = number(r[0][1]);
        _artist = r[0][2];
        _visible = r[0][3] == "t";
    }
}

void Track::setTitle(const std::string &nTitle){
    DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(_id), nTitle);
    _title = nTitle;
}

std::string Track::url(int id){
    std::stringstream s;
    s << "/track/" << id;
    return s.str();
}

std::string Track::artUrl() const{
    string path = eqbeatsDir() + "/art/" + number(_id);
    if(access(path.c_str(), R_OK))
        return std::string();
    return (std::string) "/track/" + number(_id) + "/art";
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
        "INSERT INTO tracks (user_id, title, date, license) VALUES "
        "("+number(nArtistId)+", $1, 'now', 'None specified') "
        "RETURNING id", nTitle);
    if(r.empty()) return t;
    return Track(number(r[0][0]), nTitle, nArtistId, User(nArtistId).name(), false);
}

void Track::remove(){
    DB::query("DELETE FROM tracks WHERE id = " + number(_id));
    DB::query("DELETE FROM comments WHERE type = 'track' AND ref = " + number(_id));
    string base = eqbeatsDir() + "/tracks/" + number(_id) + ".";
    string path = base + "mp3";
    unlink(path.c_str());
    path = base + "ogg";
    unlink(path.c_str());
    path = base + "flac";
    unlink(path.c_str());
    Art art(_id);
    if(art) unlink(art.filepath().c_str());
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
        tracks[i] = Track(number(r[i][0]), r[i][1], number(r[i][2]), r[i][3], r[i][4]=="t");
    return tracks;
}

#define SEL (std::string) "SELECT tracks.id, tracks.title, tracks.user_id, users.name, tracks.visible FROM tracks CROSS JOIN users " \
               "WHERE tracks.user_id = users.id AND "

std::vector<Track> Track::byArtist(int sArtistId, bool all){
    return resultToVector(DB::query(SEL + "tracks.user_id = " + number(sArtistId)
        + (all? "" : " AND tracks.visible = 't'") + " ORDER BY date DESC "));
}

std::vector<Track> Track::search(const std::string &q){
    return resultToVector(DB::query(SEL + "visible = 't' "
        "AND to_tsvector('english', tracks.title) @@ plainto_tsquery('english', $1) " + " ORDER BY date DESC ", q));
}

std::vector<Track> Track::latest(int n){
    return resultToVector(DB::query(SEL + "visible = 't' ORDER BY date DESC LIMIT " + number(n)));
}

std::vector<Track> Track::random(int n){
    return resultToVector(DB::query(SEL + "visible = 't' ORDER BY random() LIMIT " + number(n)));
}

std::vector<Track> Track::popular(int n){
    return resultToVector(DB::query(SEL + "visible = 't' ORDER BY hits DESC LIMIT " + number(n)));
}
