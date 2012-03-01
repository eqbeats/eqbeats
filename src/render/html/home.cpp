#include "home.h"
#include "feed.h"
#include "page.h"
#include "forms.h"
#include "track.h"
#include "../render.h"
#include "../../news.h"
#include "../../flag.h"
#include "../../number.h"
#include "../../track.h"
#include "../json.h"

using namespace Render;

void ticker(){
    // News
    std::vector<News> news = News::recent(7);
    if(news.empty()) news = News::latest(1);
    o << "<div id=\"newsticker\"><img src=\"/static/newspaper.png\" alt=\"news\" /> <b>Latest news</b>: <a href=\"" + news[0].url() + "\">" + news[0].title() + "</a></div>";
    if(news.size() > 1){
        o << "<script>var news = [";
        for(std::vector<News>::iterator i = news.begin(); i != news.end(); i++){
            if(i != news.begin()) o << ",";
            o << "{id: " + number(i->id()) + ", title: " + Json::jstring(i->title()) + "}";
        }
        o << "];</script>"
             "<script src=\"/static/ticker.js\"></script>";
    }
}

void Html::home(){
    header("", atomFeed("/tracks/latest/atom"));
    // Head
    ticker();
    o << "<div class=\"search\">"
         "<img src=\"/static/magnifier.png\" alt=\"\" /> Search for tracks : ";
    searchForm("/tracks/search");
    o << "</div>"
    // Tracks
         "<div class=\"leftcol\">"
         "<h3><a href=\"/tracks/latest\">Latest</a> ";
    feedIcon("/tracks/latest/atom");
    o << "</h3>";
    trackList(Track::latest(10));
    o << "<a class=\"more\" href=\"/tracks/latest\">More</a>"
         "</div><div class=\"rightcol\">"
         "<h3>Featured</h3>";
    trackList(Track::featured(10));
    o << "</div><div class=\"leftcol\">"
         "<h3><a href=\"/tracks/random\">Random</a></h3>";
    trackList(Track::random(10));
    o << "<a class=\"more\" href=\"/tracks/random\">More</a>"
         "</div><div class=\"rightcol\">"
    // Categories
         "<h3>Categories</h3>"
         "<ul>";
    Flags cs = Flags::all().categories();
    for(Flags::const_iterator i=cs.begin(); i!=cs.end(); i++)
        o << "<li><a href=\"" << i->url() << "\">" << i->name() << "</a></li>";
    o << "</ul>"
         "<h3>Partners</h3>"
         "<a href=\"http://ponify.me\" target=\"_blank\" class=\"partner\"><img src=\"/static/cr.png\" alt=\"Celestia Radio\" /></a>"
         "</div><div style=\"clear:both;\"></div>";
    footer();
}
