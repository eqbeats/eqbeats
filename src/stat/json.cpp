#include "stat.h"
#include <core/db.h>
#include <hiredis/hiredis.h>

std::string getJson(const char *format, int id){
    redisReply *reply = (redisReply*) redisCommand(DB::redis(), format, id);
    if(!reply || reply->type != REDIS_REPLY_ARRAY)
        return "[]";

    std::string out = "[\n";
    redisReply *element;
    for(int i = 0; i < reply->elements; i++){
        element = reply->element[i];
        if(element->type != REDIS_REPLY_STRING)
            continue;
        out += element->str;
        if(i != reply->elements - 1)
            out += ',';
        out += '\n';
    }
    out += "]\n";

    freeReplyObject(reply);
    return out;
}

std::string Stat::getTrackJson(int tid){
    return getJson("LRANGE stat:track:%d 0 -1", tid);
}

std::string Stat::getUserJson(int uid){
    return getJson("LRANGE stat:user:%d 0 -1", uid);
}
