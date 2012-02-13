#include <sstream>
#include "../html.h"
#include "../../news.h"
#include "../../utils.h"
#include "../json.h"

std::string newsTicker(){
    std::vector<News> news = News::recent(7);
    if(news.empty()) return std::string();
    std::string s = "<div class=\"newsticker\"><img src=\"/static/newspaper.png\" alt=\"news\" /> <b>Latest news</b>: <a href=\"" + news[0].url() + "\">" + news[0].title() + "</a></div>";
    if(news.size() > 1){
        s += "<script>var news_entries = [";
        for(std::vector<News>::iterator i = news.begin(); i != news.end(); i++){
            if(i != news.begin()) s += ',';
            s += "{id: " + number(i->id()) + ", title: " + Json::jstring(i->title()) + "}";
        }
        s += "];</script><script src=\"/static/ticker.js\"></script>";
    }
    return s;
}

std::string Html::home(){
    std::stringstream s;
    s << header("", atomFeed("/tracks/latest/atom"))
      << newsTicker()
      << "<div class=\"search\">"
      << "<img src=\"/static/magnifier.png\" /> Search for tracks : "
      << searchForm("/tracks/search")
      << "</div>"
      << "<div class=\"leftcol\">"
      << "<h3><a href=\"/tracks/latest\">Latest</a> " + feedIcon("/tracks/latest/atom") + "</h3>"
      << trackList(Track::latest(10))
      << "<a class=\"more\" href=\"/tracks/latest\">More</a>"
      << "</div><div class=\"rightcol\">"
      << "<h3><a href=\"/tracks/popular\">Popular</a></h3>"
      << trackList(Track::popular(10))
      << "<a class=\"more\" href=\"/tracks/popular\">More</a>"
      << "</div><div class=\"leftcol\">"
      << "<h3><a href=\"/tracks/random\">Random</a></h3>"
      << trackList(Track::random(10))
      << "<a class=\"more\" href=\"/tracks/random\">More</a>"
      << "</div><div class=\"rightcol\">"
      << "<h3>Categories</h3>"
      << "<ul>";
    std::vector<Category> cs = Category::list();
    for(std::vector<Category>::const_iterator i=cs.begin(); i!=cs.end(); i++)
        s << "<li><a href=\"" << i->url() << "\">" << i->name() << "</a></li>";
    s << "</ul></div><div style=\"clear:both;\"></div>" << footer();
    return s.str();
}
