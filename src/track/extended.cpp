#include "extended.h"
#include <core/db.h>
#include <text/text.h>

ExtendedTrack::ExtendedTrack(int tid){

    id = 0;
    if(tid<=0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date,"
        " tracks.notes, tracks.airable, tracks.license, array_to_string(tracks.tags, ',') FROM tracks, users "
        "WHERE tracks.id = " + number(tid) + " AND tracks.user_id = users.id");

    if(!r.empty()){

        id = tid;
        title = r[0][0];
        artist = User(number(r[0][1]), r[0][2]);
        visible = r[0][3] == "t";
        date = r[0][4];

        // Ext
        notes = r[0][5];
        airable = r[0][6] == "t";
        license = r[0][7];

        // Tags
        std::string tstr = r[0][8];
        std::string buf;
        for(std::string::const_iterator i=tstr.begin(); i!=tstr.end(); i++){
            if(*i == ','){
                if(!buf.empty()){
                    tags.push_back(buf);
                    buf.clear();
                }
            }
            else buf += *i;
        }
        if(!buf.empty()) tags.push_back(buf); // last tag

    }

}

void ExtendedTrack::fill(Dict *d) const{
    Track::fill(d);
    d->SetValueAndShowSection("NOTES", notes, "HAS_NOTES");
    // Tags
    if(!tags.empty()){
        d->ShowSection("HAS_TAGS");
        for(std::vector<std::string>::const_iterator i=tags.begin(); i!=tags.end(); i++)
            d->SetValueAndShowSection("TAG", *i, "TAG");
    }
    // License
    if(license == "Copyright")
        d->ShowSection("COPYRIGHT");
    else
        d->SetValueAndShowSection("LICENSE", license, "OTHER_LICENSE");
}
