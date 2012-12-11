#include "event.h"
#include <account/session.h>
#include <core/db.h>
#include <text/text.h>

void Event::push(){
    DB::query((std::string)
        "INSERT INTO events (type, target_id, target_name, source_id, source_name, track_id, track_title, message, date)"
        "VALUES ("
            "'" + (type==Publish?"publish":type==Comment?"comment":type==Favorite?"favorite":"follow") + "', " +
            number(target.id) + ", $1, " +
            number(source.id) + ", $2, " +
            number(track.id) + ", $3, "
            "$4, 'now')",
        target.name, source.name, track.title, message);

}

void Event::fill(Dict *d) const{
    Dict *src = d->AddSectionDictionary("SOURCE");
    if(source) source.fill(src);
    else src->SetValueAndShowSection("NAME", source.name.empty() ? "Anonymous" : source.name, "GUEST");
    target.fill(d->AddSectionDictionary("TARGET"));
    d->SetValue("UTC_DATE", formatTime(date, "%F %R"));
    d->SetValue("FUZZY_TIME", fuzzyTime(date));
    if(track){
        Dict *trackDict = d->AddSectionDictionary("TRACK");
        trackDict->SetIntValue("TID", track.id);
        trackDict->SetValue("TITLE", track.title);
    }
    else
        d->ShowSection("ON_USER");
    d->ShowSection( type == Publish ? "PUBLISH" : type == Comment ? "COMMENT"
                  : type == Favorite ? "FAVORITE" : "FOLLOW");
    d->SetValue("MESSAGE", message);
}

// EventList

EventList::EventList(std::string cond, int limit){
    DB::Result r = DB::query("SELECT type, source_id, source_name, target_id, target_name, track_id, track_title, message, date AT TIME ZONE 'UTC', id FROM events" + (cond.empty()?"":" WHERE "+cond) + " ORDER BY date DESC" + (limit?" LIMIT "+number(limit):""));
    resize(r.size());
    for(unsigned i=0; i<r.size(); i++){
        at(i).type = r[i][0] == "publish"  ? Event::Publish
                   : r[i][0] == "comment"  ? Event::Comment
                   : r[i][0] == "favorite" ? Event::Favorite : Event::Follow;
        at(i).source = User(number(r[i][1]), r[i][2]);
        at(i).target = User(number(r[i][3]), r[i][4]);
        at(i).track.id = number(r[i][5]);
        at(i).track.title = r[i][6];
        at(i).message = r[i][7];
        at(i).date = r[i][8];
        at(i).id = number(r[i][9]);
    }
}

EventList EventList::user(const User &u, int limit){
    return EventList("(source_id = " + number(u.id) + " OR target_id = " + number(u.id)
        + (u == Session::user() ? " OR source_id IN (SELECT ref FROM favorites WHERE type = 'artist' AND user_id = "+number(u.id)+")":
            " AND type = 'comment'")
            + ") AND track_id NOT IN (select id FROM tracks WHERE visible = 'f')"
            , limit);
}

EventList EventList::track(const Track &t){
    return EventList("track_id = " + number(t.id));
}

Dict* EventList::fill(Dict *d, const std::string &section, bool showForm) const{
    Dict *sub = d->AddIncludeDictionary(section);
    sub->SetFilename("html/events.tpl");
    Session::fill(sub);
    if(empty())
        sub->ShowSection("NO_EVENT");
    for(const_iterator i=begin(); i!=end(); i++)
        i->fill(sub->AddSectionDictionary("EVENT"));
    if(showForm)
        sub->ShowSection("FORM");
    return sub;
}
