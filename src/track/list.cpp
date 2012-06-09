#include "list.h"
#include <core/db.h>
#include <text/text.h>

#include <algorithm>
#include <stdio.h>

#define FIELDS "tracks.id, tracks.title, tracks.user_id, users.name, tracks.date, tracks.visible"
#define TABLES "users, tracks"
#define JOIN "tracks.user_id = users.id"
#define JOIN_VISIBLE JOIN " AND tracks.visible='t'"

TrackList::TrackList(const std::string &query_fmt, bool all){
    // Query
    char *query = (char*) malloc(query_fmt.size() + strlen(FIELDS) + strlen(TABLES) + strlen(JOIN_VISIBLE));
    sprintf(query, query_fmt.c_str(), FIELDS, TABLES, all?JOIN:JOIN_VISIBLE);
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
        Dict *player_d = i->player(track_d);
        player_d->SetValue("LIST", section);
        player_d->SetIntValue("COUNT", n++);
    }
    return list_d;
}


TrackList Tracks::latest(int n, int offset){
    return TrackList(
        "SELECT %s FROM %s WHERE %s ORDER BY date desc "
        "LIMIT " + number(n) + " OFFSET " + number(offset));
}

TrackList Tracks::featured(int n){
    return TrackList(
        "SELECT %s FROM %s, featured_tracks "
        "WHERE %s AND featured_tracks.track_id = tracks.id "
        "ORDER BY featured_tracks.date DESC LIMIT " + number(n));
}

TrackList Tracks::random(int n){
    return TrackList(
        "SELECT %s FROM %s WHERE %s ORDER BY random() LIMIT " + number(n));
}
