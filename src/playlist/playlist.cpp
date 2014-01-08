#include "playlist.h"
#include <account/session.h>
#include <core/db.h>
#include <text/text.h>

using namespace std;

Playlist::Playlist(int nId){
    DB::Result r = DB::query("SELECT users.id, users.name, playlists.name, array_length(playlists.track_ids, 1), playlists.description FROM playlists, users WHERE users.id = playlists.user_id AND playlists.id = " + number(nId));
    _id = 0;
    if(r.size() == 0) return;
    _id = nId;
    _author = User(number(r[0][0]), r[0][1]);
    _name = r[0][2];
    _length = number(r[0][3]);
    _description = r[0][4];
}

std::string Playlist::url() const{
    return "/playlist/" + number(_id);
}

TrackList Playlist::tracks(){
    return TrackList(
        "WITH playlist AS "
            "(SELECT unnest AS track_id, row_number() OVER () AS pos "
             "FROM unnest(coalesce((select track_ids FROM playlists where id = " + number(_id) + ")))) "
        "SELECT %s FROM %s, playlist WHERE %s AND tracks.id = playlist.track_id ORDER BY playlist.pos"
    , true);
}

void Playlist::add(unsigned tid){
    if(!_id || !tid) return;
    DB::query("UPDATE playlists SET track_ids = track_ids || " + number(tid) + " WHERE id=" + number(_id));
}

void Playlist::remove(unsigned pos){
    DB::Result r = DB::query("SELECT unnest(track_ids) FROM playlists WHERE id=" + number(_id));
    string ids;
    bool change = false;
    for(unsigned i=0; i<r.size(); i++){
        if(i != pos){
            if(!ids.empty()) ids += ',';
            ids += r[i][0];
        }
        else change = true;
    }
    if(change)
        DB::query("UPDATE playlists SET track_ids=ARRAY[" + ids + "]::int[] WHERE id=" + number(_id));
}

void Playlist::swap(unsigned i, unsigned j){
    if(i == j) return;
    DB::Result r = DB::query("SELECT unnest(track_ids) FROM playlists WHERE id=" + number(_id));
    if(i>=r.size() || j>=r.size()) return;
    string ids;
    for(unsigned k=0; k<r.size(); k++){
        if(!ids.empty()) ids += ',';
        ids += r[k==j?i:k==i?j:k][0];
    }
    DB::query("UPDATE playlists SET track_ids=ARRAY[" + ids + "]::int[] WHERE id=" + number(_id));
}

void Playlist::removeTrack(int tid){
    DB::Result r = DB::query("SELECT id FROM playlists WHERE " + number(tid) + " = ANY(track_ids)");
    for(DB::Result::const_iterator i=r.begin(); i!=r.end(); i++){
        std::string ids;
        DB::Result ts = DB::query("SELECT unnest(track_ids) FROM playlists WHERE id=" + (*i)[0]);
        for(DB::Result::const_iterator t=ts.begin(); t!=ts.end(); t++){
            if(number((*t)[0]) != tid){
                if(!ids.empty()) ids += ',';
                ids += (*t)[0];
            }
        }
        DB::query("UPDATE playlists SET track_ids=ARRAY[" + ids + "]::int[] WHERE id=" + (*i)[0]);
    }
}

std::vector<Playlist> Playlist::forUser(const User &u){
    DB::Result r = DB::query("SELECT id, name, array_length(track_ids, 1), description FROM playlists WHERE user_id = " + number(u.id) + " ORDER BY name ASC");
    std::vector<Playlist> playlists(r.size());
    for(unsigned i=0; i < r.size(); i++)
        playlists[i] = Playlist(number(r[i][0]), u, number(r[i][2]), r[i][1], r[i][3]);
    return playlists;
}

void Playlist::fill(Dict *d) const{
    d->SetIntValue("PLAYLIST_ID", _id);
    d->SetFormattedValue("TRACK_COUNT", "%d track%s", _length, _length>1?"s":"");
    d->SetValue("PLAYLIST_NAME", _name);
    d->SetValue("PLAYLIST_URL", url());
    d->SetValueAndShowSection("DESCRIPTION", _description, "HAS_DESCRIPTION");
    _author.fill(d);
}
