#include <string.h>
#include <sstream>
#include "track.h"
#include "user.h"
#include "number.h"
#include "db.h"
#include "art.h"
#include "repl.h"
#include "path.h"

Repl hitsd;

using namespace std;

Track::Track(int id){
    _id = 0;
    _artistId = 0;
    if(id<=0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date FROM tracks, users "
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

void Track::setTitle(const string &nTitle){
    if(_title == nTitle) return;
    DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(_id), nTitle);
    _title = nTitle;
}

string Track::url(int id){
    stringstream s;
    s << "/track/" << id;
    return s.str();
}

void Track::setVisible(bool v){
    DB::query("UPDATE tracks SET visible = '" + (string)(v?"t":"f") + "' WHERE id = " + number(_id));
    _visible = v;
}

string Track::url(Format f) const{
    string format = f==Vorbis? "vorbis" : "mp3";
    return (string) "/track/" + number(_id) + "/" + format;
}

Track Track::create(int nArtistId, const string &nTitle){
    Track t;
    DB::Result r = DB::query(
        "INSERT INTO tracks (user_id, title, date) VALUES "
        "("+number(nArtistId)+", $1, 'now') "
        "RETURNING id, date", nTitle);
    if(r.empty()) return t;
    return Track(number(r[0][0]), nTitle, nArtistId, User(nArtistId).name(), false, r[0][1]);
}

void Track::remove(){
    DB::query("DELETE FROM contest_submissions WHERE track_id = " + number(_id));
    DB::query("DELETE FROM featured_tracks WHERE track_id = " + number(_id));
    DB::query("DELETE FROM votes WHERE track_id = " + number(_id));
    DB::query("DELETE FROM comments WHERE type = 'track' AND ref = " + number(_id));
    DB::query("DELETE FROM favorites WHERE type = 'track' AND ref = " + number(_id));
    DB::query("DELETE FROM tracks WHERE id = " + number(_id));
    _id = 0;
}

void initHitsd(){
    if(!hitsd){
        string path = eqbeatsDir() + "/hitsd.sock";
        hitsd = Repl(path.c_str());
    }
}

int Track::getHits() const{
    initHitsd();
    return number(hitsd.exec("get " + number(_id)));
}
int Track::hit(){
    initHitsd();
    return number(hitsd.exec("increment " + number(_id)));
}

vector<Track> Track::resultToVector(const DB::Result &r){
    vector<Track> tracks(r.size());
    for(unsigned i=0; i<r.size(); i++)
        tracks[i] = Track(number(r[i][0]), r[i][1], number(r[i][2]), r[i][3], r[i][4]=="t", r[i][5]);
    return tracks;
}

vector<Track> Track::select(const char *tables, const string cond, const char *order, bool all, int limit, int offset){
    string q = "SELECT tracks.id, tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date FROM tracks, users";
    if(tables) q += (string) "," + tables;
    q += " WHERE tracks.user_id = users.id";
    if(!all) q += " AND tracks.visible='t'";
    if(!cond.empty()) q += " AND " + cond;
    q += (string) " ORDER BY " + order;
    if(limit) q += " LIMIT " + number(limit);
    if(offset) q += " OFFSET " + number(offset);
    return resultToVector(DB::query(q));
}

vector<Track> listTracks(const char *order, int limit, int offset=0){
    return Track::select(0, "", order, false, limit, offset);
}

vector<Track> Track::latest(int n, int offset){ return listTracks("date DESC", n, offset); }
vector<Track> Track::random(int n){ return listTracks("random()", n); }

vector<Track> Track::featured(int n){
    return Track::select("featured_tracks", "featured_tracks.track_id = tracks.id", "featured_tracks.date DESC", false, n);
}

vector<Track> Track::favorites(int uid){
    return select("favorites", 
        "favorites.type = 'track'"
        " AND favorites.ref = tracks.id "
        " AND favorites.user_id = " + number(uid),
        "users.name ASC");
}

#define SEL (string) ("SELECT tracks.id, tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date " \
                            "FROM tracks, users WHERE tracks.user_id = users.id AND tracks.visible = 't' ")

vector<Track> Track::search(const string &q){
    if(q.empty()) return vector<Track>();
    vector<string> p;
    istringstream in(q);
    string buf;
    string sql;
    while(in){
        in >> buf;
        p.push_back("%"+buf+"%");
        sql += " AND (tracks.title ILIKE $" + number(p.size()) + " OR users.name ILIKE $" + number(p.size()) + ")";
    }
    return resultToVector(DB::query(SEL + sql, p));
}

vector<Track> Track::exactSearch(const string &qartist, const string &qtitle){
    if(qtitle.empty()||qartist.empty()) return vector<Track>();
    return resultToVector(DB::query(SEL + "AND users.name = $1 AND tracks.title = $2", qartist, qtitle));
}

vector<Track> Track::byTag(const std::string &tag){
    return resultToVector(DB::query(SEL + "AND $1 = ANY(tracks.tags) ORDER BY tracks.date DESC", tag));
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

ExtendedTrack::ExtendedTrack(int id){
    _id = 0;
    _artistId = 0;
    if(id<=0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date,"
        " tracks.notes, tracks.airable, array_to_string(tracks.tags, ',') FROM tracks, users "
        "WHERE tracks.id = " + number(id) + " AND tracks.user_id = users.id");
    if(!r.empty()){
        _id = id;
        _title = r[0][0];
        _artistId = number(r[0][1]);
        _artist = r[0][2];
        _visible = r[0][3] == "t";
        _date = r[0][4];
        // Ext
        _notes = r[0][5];
        _airable = r[0][6][0] == 't';
        // Tags
        string tstr = r[0][7];
        string buf;
        for(string::const_iterator i=tstr.begin(); i!=tstr.end(); i++){
            if(*i == ','){
                if(!buf.empty()){
                    _tags.push_back(buf);
                    buf.clear();
                }
            }
            else buf += *i;
        }
        if(!buf.empty()) _tags.push_back(buf); // last tag
    }
}

void ExtendedTrack::setAirable(bool air){
    if(air == _airable) return;
    DB::query("UPDATE tracks SET airable = '" + (string)(air?"t":"f") + "' WHERE id = " + number(_id));
    _airable = air;
}

void ExtendedTrack::setNotes(const string &nNotes){
    if(_notes == nNotes) return;
    DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(_id), nNotes);
    _notes = nNotes;
}

void ExtendedTrack::setTags(const string &nTags){
    DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(_id), nTags);
    // Doesn't update _tags!
}
