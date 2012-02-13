#include "../html.h"
#include "../../news.h"
#include "../../utils.h"
#include "../../comment.h"
#include <sstream>

std::string Html::newsPage(int nid){
    News n(nid);
    if(!n) return notFound("News");
    return header(n.title())
         + "<div class=\"news\">"
         + "<h2>" + n.title() + "</h2>"
         + "<h4>on " + n.date() + "</h4>"
         + "<div class=\"news-contents\">" + n.getContents() + "</div>"
         + "</div>"
         + Html::comments(Comment::forNews(nid))
         + Html::commentForm(News::url(nid)+"/comment")
         + footer();
}

std::string Html::latestNews(int n){
    std::vector<News> news = News::latest(20);
    std::stringstream s;
    s << header("News");
    if(!news.empty()){
        s << "<div class=\"news\">"
          << "<h2>Latest news : <a href=\"" << news[0].url() << "\">" 
          << escape(news[0].title()) << "</a></h2>"
          << "<h4>on " + news[0].date() + "</h4>"
          << "<div class=\"news-contents\">" << news[0].getContents() << "</div>"
             "</div>"
             "<a class=\"more\" href=\"" << news[0].url() << "#comments\">"
             "Comments (" << number(Comment::countForNews(news[0].id())) << ")</a>";
        if(news.size()>1){
            s << "<div class=\"oldnews\">"
              << "<h3><img src=\"/static/newspaper-24.png\" alt=\"\" />Older news</h3>"
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
