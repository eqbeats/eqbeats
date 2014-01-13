#include "extended.h"
#include <core/db.h>
#include <core/path.h>
#include <text/text.h>
#include <misc/repl.h>
#include <hiredis/hiredis.h>
#include <log/log.h>

ExtendedTrack::ExtendedTrack(int tid){

    id = 0;
    if(tid<=0) return;

    DB::Result r = DB::query(
        "SELECT tracks.title, tracks.user_id, users.name, tracks.visible, tracks.date, extract(epoch from tracks.date),"
        " tracks.notes, tracks.airable, tracks.license, array_to_string(tracks.tags, ',') FROM tracks, users "
        "WHERE tracks.id = " + number(tid) + " AND tracks.user_id = users.id");

    if(!r.empty()){

        id = tid;
        title = r[0][0];
        artist = User(number(r[0][1]), r[0][2]);
        visible = r[0][3] == "t";
        date = r[0][4];
        timestamp = r[0][5];

        // Ext
        notes = r[0][6];
        airable = r[0][7] == "t";
        license = r[0][8];

        // Tags
        std::string tstr = r[0][9];
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
    d->SetValueAndShowSection("LICENSE", license, "HAS_LICENSE");
    d->ShowSection(airable ? "IS_AIRABLE" : "NOT_AIRABLE");
    // Tags
    if(!tags.empty()){
        d->ShowSection("HAS_TAGS");
        for(std::vector<std::string>::const_iterator i=tags.begin(); i!=tags.end(); i++)
            d->SetValueAndShowSection("TAG", *i, "TAG");
    }
    // License
    d->SetValue("LICENSE", license);
    d->ShowSection(license == "Copyright" ? "COPYRIGHT" : "OTHER_LICENSE");
}

// Hits

int runRedis(const char *format, int id){
    redisReply *reply = (redisReply*) redisCommand(DB::redis(), format, id);
    if(!reply)
        return 0;
    int hits = 0;
    if(reply->type == REDIS_REPLY_INTEGER)
        hits = reply->integer;
    else if(reply->type == REDIS_REPLY_STRING)
        hits = number((std::string) reply->str);
    freeReplyObject(reply);
    return hits;
}

int ExtendedTrack::getHits(){
    return runRedis("GET hits:%d", id);
}

int ExtendedTrack::hit(){
    return runRedis("INCR hits:%d", id);
}
