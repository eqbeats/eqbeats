#include "path.h"
#include <text/text.h>

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

    // path shorter than test?
    if(path.size() <= t.size()+2)
        return 0;

    // compare path to test
    if(path.substr(0, t.size()+2) != "/" + t + "/")
        return 0;

    // Find first slash following test path
    size_t i = path.find_first_of('/', t.size()+2);

    // set subpath to "rest"
    if(i == string::npos)
        sub = "";
    else
        sub = path.substr(i+1);

    // is there a - in the path?
    size_t i2 = path.find_first_of('-', t.size()+2);
    // if so, that's the delimiter for our number
    if(i2 < i)
        return number(path.substr(t.size()+2, i2-t.size()-2));
    else
        return number(path.substr(t.size()+2, i == string::npos ? i : i-t.size()-2));
}
