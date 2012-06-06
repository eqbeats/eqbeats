#include "list.h"
#include <core/db.h>
#include <media/art.h>
#include <text/text.h>

#include <algorithm>
#include <stdio.h>

#define FIELDS "tracks.id, tracks.title, tracks.user_id, users.name, tracks.date, tracks.visible"
#define JOIN "tracks.user_id = users.id"

TrackList::TrackList(const char *query_fmt){
    // Query
    char *query = (char*) malloc(strlen(query_fmt) + strlen(FIELDS) + strlen(JOIN) + 1);
    sprintf(query, query_fmt, FIELDS, JOIN);
    DB::Result r = DB::query(query);
    free(query);
    // Read
    resize(r.size());
    for(unsigned i=0; i<r.size(); i++){
        at(i).id = number(r[i][0]);
        at(i).title = r[i][1];
        at(i).artist = User(number(r[i][2]), r[i][3]);
        at(i).date = r[i][4];
        at(i).visible = r[i][5] == "t";
    }
}

Dict* TrackList::fill(Dict *d, std::string section){
    Dict *list_d = d->AddIncludeDictionary(section);
    list_d->SetFilename("tracklist.tpl");
    if(empty()){
        list_d->ShowSection("EMPTY");
        return list_d;
    }
    std::transform(section.begin(), section.end(), section.begin(), ::tolower);
    int n = 1;
    for(const_iterator i=begin(); i!=end(); i++){
        Dict *track_d = list_d->AddSectionDictionary("TRACK");
        i->fill(track_d);
        Art art(i->id);
        if(art) track_d->ShowSection("HAS_COVER");
        Dict *player_d = i->player(track_d);
        player_d->SetValue("LIST", section);
        player_d->SetIntValue("COUNT", n++);
    }
    return list_d;
}
