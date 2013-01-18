#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <text/text.h>
#include <core/cgi.h>
#include <cgicc/CgiEnvironment.h>

#include <log/log.h>

#include "push.h"

void pushStat(std::string type, int uid, int tid){
    bool trackme = true;
    for(int i=0; headers[i] != NULL; i++)
        if(strcmp(headers[i], "HTTP_DNT=1") == 0)
            trackme=false;
    cgicc::CgiEnvironment env = cgi.getEnvironment();
    std::string host = (trackme) ? env.getRemoteAddr() : "0.0.0.0";
    std::string msg = type + " " + number(tid) + " " + number(uid) + " " + host + " " + env.getReferrer() + "\n";
    int s = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    std::string loc = (std::string)getenv("EQBEATS_DIR") + "/udpstat.sock";
    strcpy(addr.sun_path, loc.c_str());
    sendto(s, msg.c_str(), msg.size(), 0, (struct sockaddr *)&addr, SUN_LEN(&addr));
    close(s);
}
