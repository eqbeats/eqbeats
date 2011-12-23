#include "routing.h"
#include <stdlib.h>
#include <stdio.h>

int routeTrack(const std::string &url){
    if(url.substr(0,7) != "/track/") return 0;
    return atoi(url.substr(7).c_str());
}

int routeTrackAction(const std::string &act, const std::string &url){
    if(url.substr(0,7) != "/track/") return 0;
    int i = url.find_last_of('/');
    if(url.substr(i+1) != act) return 0;
    return atoi(url.substr(7, i-7).c_str());
}

int routeUser(const std::string &url){
    if(url.substr(0,6) != "/user/") return 0;
    return atoi(url.substr(6).c_str());
}
