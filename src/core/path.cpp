#include "path.h"
#include <misc/number.h>

#include <stdlib.h>

using namespace std;

string stripSlash(const string &path){
    if(!path.empty()){
        if(path[path.size()-1] == '/')
            return path.substr(0, path.size()-1);
    }
    return path;
}

string eqbeatsDir(){
    return (string) getenv("EQBEATS_DIR");
}

int route(const string &t, string path, string &sub){
    path = stripSlash(path);
    if(path.size() <= t.size()+2) return 0;
    if(path.substr(0, t.size()+2) != "/" + t + "/") return 0;
    size_t i = path.find_first_of('/', t.size()+2);
    if(i == string::npos) sub = "";
    else sub = path.substr(i+1);
    return number(path.substr(t.size()+2, i == string::npos ? i : i-t.size()-2));
}
