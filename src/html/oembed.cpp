#include "html.h"
#include "../routing.h"
#include "../utils.h"
#include "../art.h"

using namespace std;

const char* notFound = "Status: 404 Not Found\n\n";

bool drop(const string &s, string &str){
    if(s.length() > str.length())
        return false;
    if(str.substr(0, s.length()) == s){
        str = str.substr(s.length());
        return true;
    }
    return false;
}

string extractPath(string url){
    drop("http://", url);
    drop("www.", url);
    return drop("eqbeats.org", url) ? url : string();
}

string header =
      "Content-Type: application/json+oembed\n\n"
      "{"
        "\"version\": \"1.0\",";
string footer =
        "\"provider_name\": \"Equestrian Beats\","
        "\"provider_url\": \"http://eqbeats.org/\""
      "}";

string embedH(const Track &t){
    return
        "\"title\": " + Html::jsonString(t.title()) + ","
        "\"author_name\": " + Html::jsonString(t.artist()) + ","
        "\"author_url\": \"" + eqbeatsUrl() + User::url(t.artistId()) + "\",";
}

string oEmbedTrack(int tid, int w){
    Track t(tid);
    if(!t) return notFound;
    if(!w) w = 500;
    return header +
        "\"type\": \"video\","
        "\"url\": \"" + eqbeatsUrl() + t.url() + "\","
        "\"width\": " + number(w) + ","
        "\"height\": 150,"
        "\"html: " + Html::jsonString(Html::embedTrackCode(t, w)) + ","
        + embedH(t)
        + footer;
}

string oEmbedArt(int tid){
    Track t(tid);
    Art art(tid);
    if(!t || !art) return notFound;
    return header +
        "\"type\": \"photo\","
        "\"url\": \"" + eqbeatsUrl() + art.url() + "\","
        + embedH(t)
        + footer;
}

string Html::oEmbed(const string &url, int w){
    string path = stripSlash(extractPath(url));
    if(path.empty()) return ::notFound;
    int id;
    if((id = routeId("track", path)))
        return oEmbedTrack(id, w);
    else if((id = routeAction("track", "art", path)))
        return oEmbedArt(id);
    return ::notFound;
}
