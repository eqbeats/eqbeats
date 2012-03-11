#include "player.h"
#include "escape.h"
#include "../render.h"
#include "../http.h"
#include "../../track.h"
#include "../../art.h"
#include "../../user.h"
#include "../../path.h"
#include "../../number.h"

using namespace Render;
using namespace std;

void Html::player(const Track &t, const string &list){
    Render::o <<
        "<div id=\"" << list << "-" << t.id() << "\" class=\"player\"></div>"
        "<script>"
            "registerTrack({"
                "id: \"" << list << "-" << t.id() << "\","
                "mp3: \"" << t.url(Track::MP3) <<"\","
                "vorbis: \"" << t.url(Track::Vorbis) <<"\""
            "});"
        "</script>"
        "<noscript>"
            "<audio controls>"
                "<source type=\"audio/ogg\" src=\"" << t.url(Track::Vorbis) << "\" />"
                "<source type=\"audio/mpeg\" src=\"" << t.url(Track::MP3) << "\" />"
            "</audio>"
        "</noscript>";
}

void Html::embedTrack(int tid){
    Track t(tid);
    Art art(tid);
    Http::header("text/html", t?200:404);
    o << "<html><head>"
            "<title>" << (t ? escape(t.title()) : "Track not found") << " - Equestrian Beats</title>"
            "<link rel=\"stylesheet\" type=\"text/css\" href=\"/static/player.css\" />"
        "</head><body><div id=\"player-embed\">"
        "<a href=\"" + t.url() + "\" target=\"_blank\" class=\"pic\">"
        + (art?
            "<img src=\"" + art.url(Art::Thumbnail) + "\" class=\"cover\" alt=\"Cover\"  />":
            "<img src=\"/static/logo.png\" class=\"logo\" alt=\"Equestrian Beats\" />"
        ) + "</a>";
    if(t) {
        o << "<h3><a href=\"" + t.url() + "\" target=\"_blank\">" + escape(t.title()) + "</a></h3>"
             "<h4>by <a href=\"" << t.artist().url() <<  "\" target=\"_blank\">" << escape(t.artist().name()) << "</a></h4>";
        player(t);
    }
    else o << "<h3 style=\"margin-bottom:10px;\">Track not found</h3>";
    o << "</div></body></html>";
}

string Html::embedTrackCode(const Track &t, int w){
    if(!w) w = 500;
    return
        "<iframe width=\""+number(w)+"px\" height=\"150px\" frameborder=\"0\" src=\"" + eqbeatsUrl() + t.url() + "/embed\">"
            "<a href=\"" + eqbeatsUrl() + t.url() + "\" target=\"_blank\">" + Html::escape(Html::escape(t.title())) + "</a>"
            " by <a href=\"" + eqbeatsUrl() + t.artist().url() + "\" target=\"_blank\">" + Html::escape(t.artist().name()) + "</a>"
        "</iframe>";
}
