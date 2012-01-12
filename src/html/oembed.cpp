#include "html.h"
#include "../routing.h"
#include "../utils.h"
#include "../art.h"
#include <sstream>

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

string field(const string &name, const string &val, bool xml){
    if(xml)
        return "<" + name + ">" + val + "</" + name + ">";
    return "\"" + name + "\":" + val + ",";
}

string stringField(const string &name, const string &val, bool xml){
    if(xml)
        return "<" + name + ">" + Html::escape(val) + "</" + name + ">";
    return "\"" + name + "\":" + Html::jsonString(val) + ",";
    
}

string header(bool xml){
    stringstream s;
    s << "Content-Type: " << (xml ? "text/xml" : "application/json") << "\n\n";
    if(xml)
      s << "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>"
           "<oembed>";
    else
      s << "{";
    s << stringField("version", "1.0", xml)
      << stringField("provider_name", "Equestrian Beats", xml)
      << stringField("provider_url", "http://eqbeats.org", xml);
    return s.str();
}

string footer(bool xml){
    return xml ? "</oembed>" : "}";
}

string embedH(const Track &t, bool xml){
    return stringField("title", t.title(), xml)
         + stringField("author_name", t.artist(), xml)
         + stringField("author_url", eqbeatsUrl() + User::url(t.artistId()), xml);
}

string oEmbedTrack(int tid, bool xml, int w){
    Track t(tid);
    if(!t) return notFound;
    if(!w) w = 500;
    return header(xml)
        + stringField("type", "video", xml)
        + stringField("url", eqbeatsUrl() + t.url(), xml)
        + field("width", number(w), xml)
        + field("height", "150", xml)
        + stringField("html", Html::embedTrackCode(t, w), xml)
        + embedH(t, xml)
        + footer(xml);
}

string oEmbedArt(int tid, bool xml){
    Track t(tid);
    Art art(tid);
    if(!t || !art) return notFound;
    return header(xml)
        + stringField("type", "photo", xml)
        + stringField("url", eqbeatsUrl() + art.url(), xml)
        + embedH(t, xml)
        + footer(xml);
}

string Html::oEmbed(const string &url, bool xml, int w){
    string path = stripSlash(extractPath(url));
    if(path.empty()) return ::notFound;
    int id;
    if((id = routeId("track", path)))
        return oEmbedTrack(id, xml, w);
    else if((id = routeAction("track", "art", path)))
        return oEmbedArt(id, xml);
    return ::notFound;
}
