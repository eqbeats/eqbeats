#include "oembed.h"
#include "json.h"
#include "http.h"
#include "render.h"
#include "html/escape.h"
#include "html/player.h"
#include "../path.h"
#include "../user.h"
#include "../track.h"
#include "../number.h"

using namespace Render;
using namespace std;

bool isXml;

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

void field(const string &name, const string &val, bool last=false){
    if(isXml) o << "<" + name + ">" + val + "</" + name + ">";
    else      o << Json::field(name, val, last);
}

void stringField(const string &name, const string &val, bool last=false){
    field(name, isXml ? Html::escape(val) : Json::jstring(val), last);
}

void Render::oEmbed(const string &url, bool xml, int w){
    isXml = xml;
    Track t(route("track", extractPath(url)));
    if(!t) return Http::header(404);
    if(!w) w = 500;

    Http::header(isXml ? "text/xml" : "application/json");
    o << (isXml
           ? "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?><oembed>"
           : "{" );
    stringField("version", "1.0");
    stringField("provider_name", "Equestrian Beats");
    stringField("provider_url", "http://eqbeats.org");
    stringField("type", "video");
    stringField("url", eqbeatsUrl() + t.url());
    field("width", number(w));
    field("height", "150");
    stringField("html", Html::embedTrackCode(t, w));
    stringField("title", t.title());
    stringField("author_name", t.artist());
    stringField("author_url", eqbeatsUrl() + User::url(t.artistId()), true);
    o << (isXml ? "</oembed>" : "}");
}
