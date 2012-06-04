#include "extended.h"

ExtendedTrack::ExtendedTrack(int tid){

    _id = 0;
    if(id<=0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date,"
        " tracks.notes, tracks.airable, tracks.license, array_to_string(tracks.tags, ',') FROM tracks, users "
        "WHERE tracks.id = " + number(id) + " AND tracks.user_id = users.id");

    if(!r.empty()){

        id = tid;
        title = r[0][0];
        artist = User(number(r[0][1]), r[0][2]);
        visible = r[0][3] == "t";
        date = r[0][4];

        // Ext
        _notes = r[0][5];
        _airable = r[0][6] == "t";
        _license = r[0][7];

        // Tags
        string tstr = r[0][8];
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
