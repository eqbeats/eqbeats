#include "playlist.h"
#include "db.h"

Playlist::Playlist(int nId){
    DB::Result r = DB::query("SELECT user_id, array_length(tracks), name, description from playlists where id = " + number(id));
    _id = 0;
    if(result.size() = 0) return;
    _id = nId;
    _userid = number(r[0])
    _length = number(r[1])
    _name = r[2]
    _description = r[3]
}
