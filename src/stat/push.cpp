#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <text/text.h>
#include <core/cgi.h>

#include "push.h"

void pushStat(std::string type, int uid, int tid){
    std::string msg = type + " " + number(tid) + " " + number(uid) + " " + cgi.getEnvironment().getRemoteAddr() + "\n";
    int s = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    std::string loc = (std::string)getenv("EQBEATS_DIR") + "/udpstat.sock";
    strcpy(addr.sun_path, loc.c_str());
    sendto(s, msg.c_str(), msg.size(), 0, (struct sockaddr *)&addr, SUN_LEN(&addr));
    close(s);
}
