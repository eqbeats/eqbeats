#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <text/text.h>
#include <core/cgi.h>
#include <cgicc/CgiEnvironment.h>
#include <misc/hash.h>
#include <log/log.h>
#include <hiredis/hiredis.h>
#include <core/db.h>

#include "stat.h"

#include <log/log.h>

int Stat::push(std::string type, int uid, int tid){
    cgicc::CgiEnvironment env = cgi.getEnvironment();
    std::string host = env.getRemoteAddr();

    redisReply* r;

    r = (redisReply*) redisCommand(DB::redis(), "INCR stat:user:%d:%s", uid, type.c_str());
    if(!r) return 0;
    int ret = r->type == REDIS_REPLY_INTEGER ? r->integer : 0;
    freeReplyObject(r);

    if(tid > 0){
        r = (redisReply*) redisCommand(DB::redis(), "INCR stat:track:%d:%s", tid, type.c_str());
        if(!r) return ret;
        ret = r->type == REDIS_REPLY_INTEGER ? r->integer : 0;
        freeReplyObject(r);

        char day[11];
        time_t t = time(NULL);
        strftime(day, 11, "%F", gmtime(&t));

        DB::blindRedisCommand("HINCRBY stat:track:%d:%s:daily %s 1", tid, type.c_str(), day);

        DB::blindRedisCommand("WATCH stat:track:%d:%s:seen:%s:%s", tid, type.c_str(), day, host.c_str());
        r = (redisReply*)redisCommand(DB::redis(), "GET stat:track:%d:%s:seen:%s:%s", tid, type.c_str(), day, host.c_str());
        if(!r) return ret;
        if(r->type == REDIS_REPLY_NIL){
            DB::blindRedisCommand("MULTI");
            DB::blindRedisCommand("INCR stat:track:%d:%s:unique", tid, type.c_str());
            DB::blindRedisCommand("HINCRBY stat:track:%d:%s:daily:unique %s 1", tid, type.c_str(), day);
            DB::blindRedisCommand("SETEX stat:track:%d:%s:seen:%s:%s 86400 1", tid, type.c_str(), day, host.c_str());
            DB::blindRedisCommand("EXEC");
        }
        else
            DB::blindRedisCommand("UNWATCH");
        freeReplyObject(r);

        DB::blindRedisCommand("ZINCRBY stat:track:%d:referrers 1 %s", tid, env.getReferrer().c_str());
    }
    return ret;
}
