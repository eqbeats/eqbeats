#include "../db.h"
#include "../track.h"
#include "../user.h"
#include "../number.h"
#include <iostream>

using namespace std;

string field(const string &name, const string &val, bool last=false){
    return "\"" + name + "\":" + val + (last?"":",");
}

string jstring(const string &str){
    string buf;
    for(string::const_iterator i=str.begin(); i!=str.end(); i++){
        if(*i == '"') buf += "\\\"";
        else if(*i == '\n') buf += "\\n";
        else buf += *i;
    }
    return "\"" + buf + "\"";
}

int main(int argc, char **argv){
    if(argc != 3){
        cerr << "Usage: nowplaying.json ARTIST TRACK" << endl;
        return 1;
    }
    DB::connect();
    User u(argv[1]);
    std::string tid;
    if(u){
        DB::Result r = DB::query("SELECT id FROM tracks WHERE user_id=" + number(u.id()) + " AND title=$1",argv[2]);
        if(!r.empty()) tid = r[0][0];
    }
    cout << "{";
    if(!tid.empty()) cout << field("id",tid);
    cout << field("title", jstring(argv[2]))
         << field("artist"
            , "{" + (u?field("id",number(u.id())):"") + field("name",jstring(argv[1]),true) + "}"
            , true)
         << "}";
    DB::close();
    return 0;
}
