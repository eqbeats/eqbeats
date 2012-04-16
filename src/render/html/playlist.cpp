#include "playlist.h"
#include "track.h"
#include "page.h"
#include "escape.h"
#include "../render.h"
#include "../../playlist.h"
#include "../../session.h"
#include "../../user.h"
#include "../../number.h"

using namespace Render;

std::string buttons(int id, int pos, int length){
    return
        "<div class=\"buttons\">"
        "<form method=\"POST\" action=\"/playlist/"+number(id)+"/remove\"><button name=\"item\" value = \""+number(pos)+"\" title=\"Remove this track from the playlist.\"><img src=\"/static/icons/music-minus.png\" alt=\"Remove\" /></button></form>" +
        (pos != 0?"<form method=\"POST\" action=\"/playlist/"+number(id)+"/move\"><button name=\"item\" value = \""+number(pos)+"\"><img src=\"/static/icons/arrow-up.png\" alt=\"Move up\" /></button><input type=\"hidden\" name=\"dir\" value=\"up\"/></form>":"<form style=\"width: 16px; height: 16px;\"></form>") +
        (pos != length - 1?"<form method=\"POST\" action=\"/playlist/"+number(id)+"/move\"><button name=\"item\" value = \""+number(pos)+"\"><img src=\"/static/icons/arrow-down.png\" alt=\"Move down\"/></button><input type=\"hidden\" name=\"dir\" value=\"down\"/></form>":"") +
        "</div>";
}

void Html::playlistPage(int id){
    Playlist p(id);
    if(!p) return Html::notFound("Playlist");
    Html::header(p.name());
    o << "<div class=\"playlist\">"
         "<h2>" << escape(p.name()) << "</h2>"
         "<h4>by <a href=\"" << p.author().url() <<  "\">" << escape(p.author().name()) << "</a></h4>";
    if(!p.description().empty())
        o << "<div class=\"notes\">" << format(p.description()) << "</div>";
    bool edition = (p.author() == Session::user());
    std::vector<Track> tracks = p.tracks();
    if(p.length() == 0)
        o << "<div class=\"empty tracklist\">Nothing here yet.</div>";
    else {
        int i = 0;
        o << "<ul class=\"tracklist\" id=\"tracks\">";
        for(std::vector<Track>::iterator t=tracks.begin(); t != tracks.end(); t++)
            Html::trackListEntry(*t, "playlist" + number(p.id()), edition?buttons(id, i++, p.length()):"");
        o << "</ul>";
    }
    if(edition){
        o << "<div class=\"edit\">"
             "<h3>" << icon("pencil") << " Edit</h3>"
             "<form method=\"post\" action=\"" << p.url() << "/edit\">"
             "<table>"
                "<tr><td>" << icon("rename") << " Name:" << "</td>"
                 "<td><input type=\"text\" name=\"name\" value=\"" << Html::escape(p.name()) << "\" /></td></tr>"
               "<tr><td>" << icon("card-pencil") << " Notes:<br />"
                 "<div class=\"legend\">(tags: [b]old, [u]nderline, [i]talic)</div></td>"
                 "<td><textarea name=\"desc\">" << escape(p.description()) << "</textarea></td></tr>"
               "<tr><td></td><td><input type=\"submit\" value=\"Update\" /></td></tr>"
             "</table></form>"
             "<a class=\"delete\" href=\"" << p.url() << "/delete\">Delete playlist</a>"
             "<div style=\"clear:both;\"></div>"
             "</div>";
    }
    o << "</div>";
    Html::footer();
}
