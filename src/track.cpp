#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "track.h"
#include "db.h"
#include "utils.h"

using namespace std;

Track::Track(int id){
    _id = -1;
    _artistId = -1;
    if(id<=0) return;

    string query =
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible FROM tracks CROSS JOIN users "
        "WHERE tracks.id = " + number(id) + " AND tracks.user_id = users.id";
    PGresult* result = PQexec(db, query.c_str());
    
    if (PQntuples(result) != 0){
        _id = id;
        _title = PQgetvalue(result, 0, 0);
        _artistId = atoi(PQgetvalue(result, 0, 1));
        _artist = PQgetvalue(result, 0, 2);
        _visible = PQgetvalue(result, 0, 3)[0] == 't';
    }
    PQclear(result);
}

void Track::setTitle(const std::string &nTitle){
    std::string query = 
        "UPDATE tracks SET title = '" + nTitle + "' "
        "WHERE id = " + number(_id);
    _title = nTitle;
    PQclear(PQexec(db, query.c_str()));
}

std::string Track::pageUrl(int id){
    std::stringstream s;
    s << "/track/" << id;
    return s.str();
}

void Track::setVisible(bool v){
    std::string query = 
        "UPDATE tracks SET visible = '"+
        (std::string)(v?"t" : "f") +
        "' WHERE id = " + number(_id);
    _visible = v;
    PQclear(PQexec(db, query.c_str()));
}

std::string urlHelper(const Track *t, std::string ext, std::string format){
    string path = "/srv/eqbeats/tracks/" + number(t->id()) + "." + ext;
    if(access(path.c_str(), R_OK))
        return std::string();
    return (std::string) "/track/" + number(t->id()) + "/" + format;
}

std::string Track::flacUrl()const{
    return urlHelper(this, "flac", "flac");
}

std::string Track::mp3Url()const{
    return urlHelper(this, "mp3", "mp3");
}

std::vector<Track> Track::byArtist(int sArtistId, bool all){
    string query =
        "SELECT tracks.id, tracks.title, tracks.user_id, users.name, tracks.visible FROM tracks CROSS JOIN users "
        "WHERE tracks.user_id = " + number(sArtistId) + " AND tracks.user_id = users.id " +
        (string) (all? "" : "AND tracks.visible = 't'") + " ORDER BY date DESC ";
    PGresult* result = PQexec(db, query.c_str());

    int rows = PQntuples(result);
    std::vector<Track> tracks(rows);
    for(int row=0; row<rows; row++)
        tracks[row] = Track(
            atoi(PQgetvalue(result, row, 0)), (std::string) PQgetvalue(result, row, 1),
            atoi(PQgetvalue(result, row, 2)), (std::string) PQgetvalue(result, row, 3),
            PQgetvalue(result, row, 4)[0] == 't'
            );

    PQclear(result);
    return tracks;
}
