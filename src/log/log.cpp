#include "log.h"
#include <syslog.h>

using namespace std;

void log(const std::string &msg){
    syslog(LOG_NOTICE, "%s", msg.c_str());
}
