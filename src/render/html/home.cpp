#include "home.h"
#include "feed.h"
#include "page.h"
#include "forms.h"
#include "track.h"
#include "../render.h"
#include "../../news.h"
#include "../../number.h"
#include "../../track.h"
#include "../json.h"

using namespace Render;

void ticker(){
    // News
    std::vector<News> news = News::recent(7);
    if(news.empty()) news = News::latest(1);
    o << "<div id=\"newsticker\">" << Html::icon("newspaper") << " <b>Latest news</b>: <a href=\"" << news[0].url() << "\">" << news[0].title() << "</a></div>";
    if(news.size() > 1){
        o << "<script>var news = [";
        for(std::vector<News>::iterator i = news.begin(); i != news.end(); i++){
            if(i != news.begin()) o << ",";
            o << "{id: " << number(i->id()) << ", title: " << Json::jstring(i->title()) << "}";
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
         "<h3>Featured</h3>";
    trackList(Track::featured(7), "featured");
    o << "</div><div class=\"leftcol\">"
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
