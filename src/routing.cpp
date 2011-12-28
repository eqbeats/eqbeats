#include "routing.h"
#include "utils.h"

int routeId(const std::string &t, const std::string &path){
    if(path.substr(0,t.size()+2) != "/" + t + "/") return 0;
    return number(path.substr(t.size()+2));
}

int routeAction(const std::string &t, const std::string &act, const std::string &path){
    if(path.substr(0,t.size()+2) != "/" + t + "/") return 0;
    int i = path.find_last_of('/');
    if(path.substr(i+1) != act) return 0;
    return number(path.substr(t.size()+2, i-t.size()-2));
}
