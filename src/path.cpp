#include "path.h"
#include "number.h"
#include "actions/actions.h"

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

string eqbeatsUrl(){
    return "http://" + Action::cgi->getEnvironment().getServerName();
}

string getPath(){
    return stripSlash(Action::cgi->getEnvironment().getPathInfo());
}

int route(const string &t, string path){
    path = stripSlash(path);
    if(path.substr(0,t.size()+2) != "/" + t + "/") return 0;
    return number(path.substr(t.size()+2));
}

int route(const string &t, const string &act, string path){
    path = stripSlash(path);
    if(path.substr(0,t.size()+2) != "/" + t + "/") return 0;
    int i = path.find_last_of('/', path.size() - act.size());
    if(path.substr(i+1) != act) return 0;
    return number(path.substr(t.size()+2, i-t.size()-2));
}
