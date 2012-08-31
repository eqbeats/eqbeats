#include "json.h"
#include "http.h"
#include "../track.h"
#include "../cgi.h"
#include "../account.h"
#include "../follower.h"
#include "../path.h"
#include "../number.h"
#include "../playlist.h"
#include "render.h"
#include <sstream>
#include <iostream>
#include "html/escape.h"

using namespace std;
using namespace Render;
using namespace Json;

void header(){
    o << "Access-Control-Allow-Origin: *\n";
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
      + field("link", jstring(eqbeatsUrl() + User::url(uid)), true)
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
      + field("link", jstring(eqbeatsUrl() + t.url()))
      + field("download", "{"
          + field("mp3", jstring(eqbeatsUrl() + t.url() + "/mp3"))
          + field("vorbis", jstring(eqbeatsUrl() + t.url() + "/vorbis"), true)
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

string playlistArray(const vector<Playlist> &ps){
    stringstream s;
    s << "[";
    for(vector<Playlist>::const_iterator i=ps.begin(); i!=ps.end(); i++){
        if(i != ps.begin()) s << ",";
        s << "{" << field("id", number(i->id()))
                 << field("name", jstring(i->name()))
                 << field("author", artistH(i->author().id(), i->author().name()))
                 << field("link", jstring(eqbeatsUrl() + i->url()), true) << "}";
    }
    s << "]";
    return s.str();
}

void Json::tracks(const vector<Track> &ts){
    header();
    o << tracksArray(ts);
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
      << field("playlists", playlistArray(Playlist::forUser(u)))
      << field("link", jstring(eqbeatsUrl() + u.url()), true)
      << "}";
    footer();
}

void Json::users(const vector<User> &us){
    header();
    o << "[";
    for(vector<User>::const_iterator i=us.begin(); i!=us.end(); i++){
        if(i != us.begin()) o << ",";
        o << artistH(i->id(), i->name());
    }
    o << "]";
    footer();
}

void Json::favorites(int uid){
    header();
    o << tracksArray(Track::favorites(uid));
    footer();
}

void Json::playlist(int pid){
    Playlist p(pid);
    if(!p) return Http::header(404);
    string notes = p.description();
    header();
    o << "{"
      << field("id", number(p.id()))
      << field("name", jstring(p.name()))
      << field("author", artistH(p.author().id(), p.author().name()))
      << (notes.empty() ? "" :
            field("description", jstring(notes))
          + field("html_description", jstring(Html::format(notes)))
        )
      << field("tracks", tracksArray(p.tracks()))
      << field("link", jstring(eqbeatsUrl() + p.url()), true)
      << "}";
    footer();
}
