#include "json.h"
#include "http.h"
#include "../track.h"
#include "../cgi.h"
#include "../account.h"
#include "../path.h"
#include "../number.h"
#include "render.h"
#include <sstream>
#include <iostream>
#include "html/escape.h"

using namespace std;
using namespace Render;
using namespace Json;

void header(){ 
    if(cgi("jsonp").empty())
        Http::header("application/json");
    else {
        Http::header("text/javascript");
        o << cgi("jsonp") << "(";
    }
}

void footer(){
    if(!cgi("jsonp").empty()) o << ");";
}

string Json::field(const string &name, const string &val, bool last){
    return "\"" + name + "\":" + val + (last?"":",");
}

string Json::jstring(const string &str){
    string buf;
    for(string::const_iterator i=str.begin(); i!=str.end(); i++){
        if(*i == '"') buf += "\\\"";
        else if(*i == '\n') buf += "\\n";
        else if(*i == '\r');
        else buf += *i;
    }
    return "\"" + buf + "\"";
}

string artistH(int uid, const string &name){
    return "{"
      + field("id", number(uid))
      + field("name", jstring(name))
      + field("link", jstring("http://eqbeats.org/user/" + number(uid)), true)
      + "}";
}

string trackH(const Track &t, const string &notes=string()){
    return "{"
      + field("id", number(t.id()))
      + field("title", jstring(t.title()))
      + (notes.empty() ? "" : 
            field("description", jstring(notes))
          + field("html_description", jstring(Html::format(notes)))
        )
      + field("artist", artistH(t.artist().id(), t.artist().name()))
      + field("link", jstring("http://eqbeats.org/track/" + number(t.id())))
      + field("download", "{"
          + field("mp3", jstring("http://eqbeats.org/track/" + number(t.id()) + "/mp3"))
          + field("vorbis", jstring("http://eqbeats.org/track/" + number(t.id()) + "/vorbis"), true)
          + "}", true)
      + "}";
}

void Json::track(int tid){
    ExtendedTrack t(tid);
    if(!t) return Http::header(404);
    header();
    o << trackH(t, t.notes());
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
    Account u(uid);
    if(!u) return Http::header(404);
    string about = u.about();
    header();
    o << "{"
      << field("id", number(u.id()))
      << field("name", jstring(u.name()))
      << (about.empty() ? "" : 
            field("description", jstring(about))
          + field("html_description", jstring(Html::format(about)))
        )
      << field("tracks", tracksArray(u.tracks()))
      << field("link", jstring("http://eqbeats.org/user/" + number(uid)), true)
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
