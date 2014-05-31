#include "list.h"
#include <core/db.h>
#include <text/text.h>

#include <algorithm>
#include <sstream>
#include <stdio.h>

#define FIELDS "tracks.id, tracks.title, tracks.user_id, users.name, tracks.date, extract(epoch from tracks.date), tracks.visible"
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
        at(i).timestamp = r[i][5];
        at(i).visible = r[i][6] == "t";
    }
}

Dict* TrackList::fill(Dict *d, std::string section, bool buttons) const {
    Dict *list_d = d->AddIncludeDictionary(section);
    list_d->SetFilename("html/tracklist.tpl");
    if(empty()){
        list_d->ShowSection("EMPTY");
        return list_d;
    }
    std::transform(section.begin(), section.end(), section.begin(), ::tolower);
    for(unsigned i=0; i<size(); i++){
        Dict *track_d = list_d->AddSectionDictionary("TRACK");
        if(buttons){
            track_d->ShowSection("BUTTONS");
            track_d->SetIntValue("POSITION", i);
            track_d->ShowSection(i == 0 ? "IS_FIRST" : "NOT_FIRST");
            track_d->ShowSection(i == size()-1 ? "IS_LAST" : "NOT_LAST");
        }
        at(i).fill(track_d);
        Dict *player_d = at(i).player(track_d);
        player_d->SetValue("LIST", section);
        player_d->SetIntValue("COUNT", i+1);
    }
    return list_d;
}

Dict* TrackList::fillJson(Dict *d, bool showArtist){
    Dict *item, *data;
    for(TrackList::const_iterator i=begin(); i!=end(); i++){
        item = d->AddSectionDictionary("ITEM");
        data = item->AddIncludeDictionary("DATA");
        data->SetFilename("json/track.tpl");
        if(showArtist)
            data->ShowSection("ARTIST");
        i->fill(data);
    }
    return d;
}

// Prepared queries

TrackList TrackList::search(const std::string &q){
    TrackList list;
    if(q.empty()) return list;
    std::vector<std::string> p;
    std::istringstream in(q);
    std::string buf;
    std::string sql;
    while(in.good()){
        in >> std::skipws >> buf;
        if(drop("artist:", buf)){
            p.push_back("%"+buf+"%");
            sql += " AND users.name ILIKE $" + number(p.size());
        }
        else if(drop("title:", buf)){
            p.push_back("%"+buf+"%");
            sql += " AND tracks.title ILIKE $" + number(p.size());
        }
        else if(drop("license:", buf)){
            p.push_back("%"+buf+"%");
            sql += " AND tracks.license ILIKE $" + number(p.size());
        }
        else{
            p.push_back("%"+buf+"%");
            sql += " AND (tracks.title ILIKE $" + number(p.size()) + " OR users.name ILIKE $" + number(p.size()) + ")";
        }
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

TrackList Tracks::all(int per_page, int page = 1){
    if(page <= 0) page = 1;
    return TrackList(
        "SELECT %s FROM %s WHERE %s ORDER BY id ASC" +
        (per_page > 0?" LIMIT " + number(per_page):"") + " OFFSET " + number((page-1) * per_page));
}

TrackList Tracks::latest(int n, int offset){
    return TrackList(
        "SELECT %s FROM %s WHERE %s ORDER BY date DESC "
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

TrackList Tracks::byUser(int uid, bool all){
    return TrackList(
        "SELECT %s FROM %s WHERE %s AND users.id = " + number(uid) + " ORDER BY date DESC", all);
}
