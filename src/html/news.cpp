#include "html.h"
#include "../news.h"
#include <sstream>
#include "../utils.h"

std::string Html::newsPage(int nid){
    News n(nid);
    if(!n)
        return header("News not found", 404) + footer();
    return header(n.title())
         + "<div class=\"news\">" + n.getContents() + "</div>"
         + footer();
}

std::string Html::latestNews(int n){
    std::vector<News> news = News::latest(20);
    std::stringstream s;
    s << header();
    if(!news.empty()){
        s << "<h2>Latest news : <a href=\"" << news[0].url() << "\">" 
          << news[0].title() << "</a></h2>"
          << "<div class=\"news\">" << news[0].getContents() << "</div>";
        if(news.size()>1){
            s << "<div class=\"oldnews\">"
              << "<h3>Older news</h3>"
              << "<ul>";
            for(std::vector<News>::const_iterator i=news.begin()+1; i!=news.end(); i++)
                s << "<li><a href=\"" << i->url() << "\">" << i->title() << "</a></li>";
            s << "</ul>"
              << "</div>";
        }
    }
    s << footer();
    return s.str();
}
