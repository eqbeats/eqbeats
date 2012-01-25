#include "json.h"
#include "http.h"
#include "../utils.h"
#include "../user.h"
#include <sstream>

using namespace std;
using namespace Json;

string header(){ return Http::header("application/json"); }

string Json::field(const string &name, const string &val, bool last){
    return "\"" + name + "\":" + val + (last?"":",");
}

string Json::jstring(const string &str){
    string buf;
    for(string::const_iterator i=str.begin(); i!=str.end(); i++){
        if(*i == '"') buf += "\\\"";
        else if(*i == '\n') buf += "\\n";
        else buf += *i;
    }
    return "\"" + buf + "\"";
}

string artistH(int uid, const string &name){
    return "{"
         + field("id", number(uid))
         + field("name", jstring(name), true)
         + "}";
}

string trackH(const Track &t){
    return "{"
         + field("id", number(t.id()))
         + field("title", jstring(t.title()))
         + field("artist", artistH(t.artistId(), t.artist()), true)
         + "}";
}

string Json::track(const Track &t){
   if(!t) return Http::notFound();
   return header() + trackH(t);
}

string tracksArray(const vector<Track> &ts){
    stringstream s;
    s << "[";
    for(vector<Track>::const_iterator i=ts.begin(); i!=ts.end(); i++){
        if(i != ts.begin()) s << ",";
        s << trackH(*i);
    }
    s << "]";
    return s.str();
}

string Json::tracks(const vector<Track> &ts){
    return header() + "{" + field("tracks", tracksArray(ts), true) + "}";
}

string Json::users(const vector<User> &us){
    stringstream s;
    s << header()<< "{\"users\":[";
    for(vector<User>::const_iterator i=us.begin(); i!=us.end(); i++){
        if(i != us.begin()) s << ",";
        s << artistH(i->id(), i->name());
    }
    s << "]}";
    return s.str();
}

string Json::artist(int uid){
    User u(uid);
    if(!u) return Http::notFound();
    return header()+ "{"
         + field("id", number(u.id()))
         + field("name", jstring(u.name()))
         + field("tracks", tracksArray(Track::byArtist(u.id())), true)
         + "}";
}

string Json::category(int cid){
    Category c(cid);
    if(!c) return Http::notFound();
    return header()+ "{"
        + field("name", jstring(c.name()))
        + field("tracks", tracksArray(Track::byCategory(c.id())), true)
        + "}";
}
