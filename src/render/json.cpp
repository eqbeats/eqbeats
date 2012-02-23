#include "json.h"
#include "http.h"
#include "../category.h"
#include "../track.h"
#include "../actions/actions.h"
#include "../user.h"
#include "../number.h"
#include "render.h"
#include <sstream>
#include <iostream>

using namespace std;
using namespace Render;
using namespace Json;

void header(){ 
    if(Action::cgi("jsonp").empty())
        Http::header("application/json");
    else {
        Http::header("text/javascript");
        o << Action::cgi("jsonp") << "(";
    }
}

void footer(){
    if(!Action::cgi("jsonp").empty()) o << ");";
}

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
      + field("artist", artistH(t.artist().id(), t.artist().name()), true)
      + "}";
}

void Json::track(int tid){
    Track t(tid);
    if(!t) return Http::header(404);
    header();
    o << trackH(t);
    footer();
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

void Json::tracks(const vector<Track> &ts){
    header();
    o << "{" + field("tracks", tracksArray(ts), true) + "}";
    footer();
}

void Json::artist(int uid){
    User u(uid);
    if(!u) return Http::header(404);
    header();
    o << "{"
      << field("id", number(u.id()))
      << field("name", jstring(u.name()))
      << field("tracks", tracksArray(u.tracks()), true)
      << "}";
    footer();
}

void Json::users(const vector<User> &us){
    header();
    o << "{\"users\":[";
    for(vector<User>::const_iterator i=us.begin(); i!=us.end(); i++){
        if(i != us.begin()) o << ",";
        o << artistH(i->id(), i->name());
    }
    o << "]}";
    footer();
}

void Json::category(int cid){
    Category c(cid);
    if(!c) return Http::header(404);
    header();
    o << "{"
      << field("name", jstring(c.name()))
      << field("tracks", tracksArray(c.tracks()), true)
      << "}";
    footer();
}

void Json::nowPlaying(const string &file){
    header();
    ifstream j(file.c_str());
    o << j.rdbuf();
    j.close();
    footer();
}
