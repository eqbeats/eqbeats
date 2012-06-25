#include "list.h"
#include <core/db.h>
#include <text/text.h>

#include <algorithm>
#include <sstream>
#include <stdio.h>

#define FIELDS "tracks.id, tracks.title, tracks.user_id, users.name, tracks.date, tracks.visible"
#define TABLES "users, tracks"
#define JOIN "tracks.user_id = users.id"
#define JOIN_VISIBLE JOIN " AND tracks.visible='t'"

TrackList::TrackList(const std::string &query_fmt, bool all){
    char *query = (char*) malloc(query_fmt.size() + strlen(FIELDS) + strlen(TABLES) + strlen(JOIN_VISIBLE));
    sprintf(query, query_fmt.c_str(), FIELDS, TABLES, all?JOIN:JOIN_VISIBLE);
    extract(DB::query(query));
    free(query);
}

void TrackList::extract(const DB::Result &r){
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
    list_d->SetFilename("html/tracklist.tpl");
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

// Prepared queries

TrackList TrackList::search(const std::string &q){
    TrackList list;
    if(q.empty()) return list;
    std::vector<std::string> p;
    std::istringstream in(q);
    std::string buf;
    std::string sql;
    while(in){
        in >> buf;
        p.push_back("%"+buf+"%");
        sql += " AND (tracks.title ILIKE $" + number(p.size()) + " OR users.name ILIKE $" + number(p.size()) + ")";
    }
    list.extract(DB::query("SELECT "FIELDS" FROM "TABLES" WHERE "JOIN_VISIBLE + sql, p));
    return list;
}

TrackList TrackList::exactSearch(const std::string &artist, const std::string &title){
    TrackList list;
    if(artist.empty() || title.empty()) return list;
    list.extract(DB::query(
        "SELECT "FIELDS" FROM "TABLES" WHERE "JOIN_VISIBLE" AND users.name = $1 AND tracks.title = $2", artist, title));
    return list;
}

TrackList TrackList::tag(const std::string &t){
    TrackList list;
    list.extract(DB::query(
        "SELECT "FIELDS" FROM "TABLES" WHERE "JOIN_VISIBLE" AND $1 = ANY(tracks.tags) ORDER BY tracks.date DESC", t));
    return list;
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
