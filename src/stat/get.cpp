#include "stat.h"
#include <core/db.h>
#include <hiredis/hiredis.h>
#include <text/text.h>

int Stat::get(const std::string type, int uid, int tid, bool unique){
    redisReply* r = (redisReply*) redisCommand(DB::redis(),
        ((std::string) "GET stat:" + (tid? "track:"+number(tid) : "user:"+number(uid)) + ":" + type + (unique? ":unique" : "")).c_str());
    if(!r) return -1;
    int ret = 0;
    if(r->type == REDIS_REPLY_STRING)
        ret = number(std::string(r->str, r->len));
    else if(r->type == REDIS_REPLY_INTEGER)
        ret = r->integer;
    freeReplyObject(r);
    return ret;
}

map<std::string, int> zip(redisReply* r){
    map<std::string, int> ret;
    if(!r || r->type != REDIS_REPLY_ARRAY) return ret;
    for(int i = 0; i < r->elements; i += 2){
        if(r->element[i]->type != REDIS_REPLY_STRING) continue;
        std::string key(r->element[i]->str, r->element[i]->len);
        int value = 0;
        switch(r->element[i+1]->type){
            case(REDIS_REPLY_STRING):
                value = number(std::string(r->element[i+1]->str, r->element[i+1]->len));
                break;
            case(REDIS_REPLY_INTEGER):
                value = r->element[i+1]->integer;
                break;
            default:
                continue;
        }
        ret.insert(pair<std::string, int>(key, value));
    }
    return ret;
}

map<std::string, int> Stat::getDays(const std::string type, int tid, bool unique){
    redisReply* r = (redisReply*) redisCommand(DB::redis(),
            ((std::string) "HGETALL stat:track:%d:%s:daily" + (unique? ":unique" : "")).c_str(), tid, type.c_str());
    map<std::string, int> ret;
    if(r != NULL){
        ret = zip(r);
        freeReplyObject(r);
    }
    return ret;
}

map<std::string, int> Stat::getReferrers(int tid, int limit){
    redisReply* r = (redisReply*) redisCommand(DB::redis(),
            ((std::string) "ZREVRANGE stat:track:%d:referrers 0 %d WITHSCORES").c_str(), tid, limit);
    map<std::string, int> ret;
    if(r != NULL){
        ret = zip(r);
        freeReplyObject(r);
    }
    return ret;

}

void Stat::fillMeasurement(Dict *outer, std::string name, int uid, int tid, bool unique, bool daily){
    Dict *dict = outer->AddSectionDictionary(unique?daily?"DAILY_UNIQUE_MEASUREMENT":"UNIQUE_MEASUREMENT":daily?"DAILY_MEASUREMENT":"MEASUREMENT");
    dict->SetValue("MEASUREMENT", name);
    if(!daily)
        dict->SetIntValue("VALUE", Stat::get(name, uid, tid, unique));
    else{
        map<std::string, int> days = Stat::getDays(name, tid, unique);
        for(std::map<std::string, int>::iterator day = days.begin(); day != days.end(); day++){
            Dict *inner = dict->AddSectionDictionary("DAY");
            inner->SetValue("DAY", day->first);
            inner->SetIntValue("VALUE", day->second);
        }
    }
}
