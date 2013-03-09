#include "feature.h"

Feature::Feature(unsigned int uid){
    DB::Result r = DB::query(
        "SELECT type, ref FROM user_features WHERE user_id = " + number(uid));
    if(r.empty()){
        type = None;
    } else if (r[0][0] == "track"){
        type = Track;
        tid = number(r[0][1]);
        tracklist = TrackList("SELECT %s FROM %s WHERE %s AND tracks.id = " + number(tid), true);
    } else {
        type = Playlist;
        playlist = ::Playlist(number(r[0][1]));
        tracklist = playlist.tracks();
    }
}

void Feature::fill(Dict* d) {
    if(type == None)
        d->ShowSection("NO_FEATURED");
    else {
        d->ShowSection("HAS_FEATURED");
        tracklist.fill(d, "FEATURED_TRACKS");
        if(type == Playlist){
            Dict* title = d->AddSectionDictionary("FEATURED_TITLE");
            title->SetValue("FEATURED_TITLE", playlist.name());
            title->SetValue("FEATURED_LINK", playlist.url());
        }
    }
}
