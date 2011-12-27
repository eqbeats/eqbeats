#include "routing.h"
#include "utils.h"

int routeTrack(const std::string &url){
    if(url.substr(0,7) != "/track/") return 0;
    return number(url.substr(7));
}

int routeTrackAction(const std::string &act, const std::string &url){
    if(url.substr(0,7) != "/track/") return 0;
    int i = url.find_last_of('/');
    if(url.substr(i+1) != act) return 0;
    return number(url.substr(7, i-7));
}

int routeUser(const std::string &url){
    if(url.substr(0,6) != "/user/") return 0;
    return number(url.substr(6));
}

int routeNews(const std::string &url){
    if(url.substr(0,6) != "/news/") return 0;
    return number(url.substr(6));
}
