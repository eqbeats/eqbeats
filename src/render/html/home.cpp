#include "home.h"
#include "feed.h"
#include "page.h"
#include "forms.h"
#include "track.h"
#include "escape.h"
#include "../../number.h"
#include "../../track.h"
#include "../../db.h"
#include "../render.h"
#include "../json.h"

using namespace Render;

void ticker(){
    DB::Result news = DB::query("SELECT title, url FROM ticker WHERE 'now' < expire ORDER BY expire DESC");
    if(news.empty()) news = DB::query("SELECT title, url FROM ticker ORDER BY expire DESC LIMIT 1");
    if(news.empty()) return;
    o << "<div id=\"newsticker\">" << Html::icon("newspaper")
      << " <b>Latest news</b>: <a href=\"" << Html::escape(news[0][1]) << "\">" << Html::escape(news[0][0]) << "</a></div>";
    if(news.size() > 1){
        o << "<script>var news = [";
        for(unsigned i=0; i<news.size(); i++){
            if(i) o << ",";
            o << "{title: " << Json::jstring(news[i][0]) << ", url: " << Json::jstring(news[i][1]) << "}";
        }
        o << "];</script>"
             "<script src=\"/static/ticker.js\"></script>";
    }
}

void Html::home(){
    header("", metaDescription("Publish your MLP-related tracks and listen to your favorite songs.") + atomFeed("/tracks/latest/atom"));
    // Head
    ticker();
    o << "<div class=\"search\">"
      << icon("magnifier") << " Search for tracks : ";
    searchForm("/tracks/search");
    o << "</div>"
    // Tracks
         "<div class=\"leftcol\">"
         "<h3><a href=\"/tracks/latest\">Latest</a> ";
    feedIcon("/tracks/latest/atom");
    o << "</h3>";
    trackList(Track::latest(7), "latest");
    o << "<a class=\"more\" href=\"/tracks/latest\">More</a>"
         "</div><div class=\"rightcol\">"
         "<h3><a href=\"/tracks/featured\">Featured</a> ";
    feedIcon("/tracks/featured/atom");
    o << "</h3>";
    trackList(Track::featured(7), "featured");
    o << "<a class=\"more\" href=\"/tracks/featured\">More</a>"
         "</div><div class=\"leftcol\">"
         "<h3><a href=\"/tracks/random\">Random</a></h3>";
    trackList(Track::random(3), "random");
    o << "<a class=\"more\" href=\"/tracks/random\">More</a>"
    // End
         "</div><div class=\"rightcol\">"
         "<h3>Partners</h3>"
         "<a href=\"http://ponify.me\" target=\"_blank\" class=\"partner\"><img src=\"/static/cr.png\" alt=\"Celestia Radio\" /></a>"
         "</div><div style=\"clear:both;\"></div>";
    footer();
}
