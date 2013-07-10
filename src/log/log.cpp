#include "log.h"
#include <account/session.h>
#include <syslog.h>

using namespace std;

void log(const std::string &msg){
    if(Session::user())
        syslog(LOG_NOTICE, "%s [user: %d]", msg.c_str(), Session::user().id);
    else
        syslog(LOG_NOTICE, "%s", msg.c_str());
}
