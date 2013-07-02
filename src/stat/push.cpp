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

std::string salt_;

std::string salt(){
    if(!salt_.empty())
        return salt_;
    int f = open(((std::string)getenv("EQBEATS_DIR") + "/.udpstat.salt").c_str(), O_RDONLY);
    if(f > -1){
        char buffer[10];
        read(f, buffer, 10);
        close(f);
        salt_ = std::string(buffer, 10);
        return salt_;
    }
    else {
        int urandom = open("/dev/urandom", O_RDONLY);
        char buffer[10];
        read(urandom, buffer, 10);
        close(urandom);
        salt_ = md5(std::string(buffer, 10)).substr(0,10);

        umask(0777 % 0640);
        FILE* f_ = fopen(((std::string)getenv("EQBEATS_DIR") + "/.udpstat.salt").c_str(), "w");
        fprintf(f_, "%s", salt_.c_str());
        fclose(f_);

        return salt_;
    }
}

void Stat::push(std::string type, int uid, int tid){
    bool trackme = true;
    for(int i=0; headers[i] != NULL; i++)
        if(strcmp(headers[i], "HTTP_DNT=1") == 0)
            trackme=false;
    cgicc::CgiEnvironment env = cgi.getEnvironment();
    std::string host = (trackme) ? env.getRemoteAddr() : "0.0.0.0";
    std::string addr = md5(host + salt()).substr(0,10);

    std::string entry = "{ \"type\":\"" + type + "\", "+
                        (tid > 0?"\"tid\":" + number(tid) + ", ":"")+
                        (type == "userView"?"\"uid\":" + number(uid) + ", ":"")+
                        "\"timestamp\":" + number(time(NULL)) + ", "+
                        "\"referrer\":\"" + env.getReferrer() + "\", "+
                        "\"addr\":\"" + addr + "\", "+
                        "\"unique\":-1 }";

    if(tid > 0)
        redisCommand(DB::redis(), "RPUSH stat:track:%d %s", tid, entry.c_str());
    if(uid > 0)
        redisCommand(DB::redis(), "RPUSH stat:user:%d %s", uid, entry.c_str());
}
