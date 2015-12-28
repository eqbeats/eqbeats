#include "playlist.h"
#include <core/db.h>
#include <text/text.h>

using namespace std;

Playlist::Playlist(int nId){
    DB::Result r = DB::query("SELECT users.id, users.name, users.email, playlists.name, array_length(playlists.track_ids, 1), playlists.description FROM playlists, users WHERE users.id = playlists.user_id AND playlists.id = " + number(nId));
    _id = 0;
    if(r.size() == 0) return;
    _id = nId;
    _author.id = number(r[0][0]);
    _author.name = r[0][1];
    _author.email = r[0][2];
    _name = r[0][3];
    _length = number(r[0][4]);
    _description = r[0][5];
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

std::vector<Playlist> Playlist::forUser(const User &u){
    DB::Result r = DB::query("SELECT id, name, array_length(track_ids, 1), description FROM playlists WHERE user_id = " + number(u.id) + " ORDER BY name ASC");
    std::vector<Playlist> playlists(r.size());
    for(unsigned i=0; i < r.size(); i++)
        playlists[i] = Playlist(number(r[i][0]), u, number(r[i][2]), r[i][1], r[i][3]);
    return playlists;
}

void Playlist::fill(Dict *d) const{
    d->SetIntValue("PLAYLIST_ID", _id);
    d->SetIntValue("PLAYLIST_TRACK_COUNT", _length);
    d->SetFormattedValue("TRACK_COUNT", "%d track%s", _length, _length>1?"s":"");
    d->SetValue("PLAYLIST_NAME", _name);
    d->SetValue("PLAYLIST_URL", url());
    d->SetValueAndShowSection("DESCRIPTION", _description, "HAS_DESCRIPTION");
    _author.fill(d);
}
