#include "news.h"
#include "page.h"
#include "comment.h"
#include "escape.h"
#include "../render.h"
#include "../../news.h"
#include "../../number.h"
#include "../../comment.h"

using namespace Render;

void Html::newsPage(int nid){
    News n(nid);
    if(!n) return notFound("News");
    header(n.title());
    o << "<div class=\"news\">"
      << "<h2>" << n.title() << "</h2>"
      << "<h4>on " << n.date() << "</h4>"
      << "<div class=\"news-contents\">" << n.getContents() << "</div>"
      << "</div>";
    Html::comments(Comment::forNews(nid));
    Html::commentForm(News::url(nid)+"/comment");
    footer();
}

void Html::latestNews(int n){
    std::vector<News> news = News::latest(20);
    header("News");
    if(!news.empty()){
        o << "<div class=\"news\">"
          << "<h2>Latest news : <a href=\"" << news[0].url() << "\">" 
          << escape(news[0].title()) << "</a></h2>"
          << "<h4>on " + news[0].date() + "</h4>"
          << "<div class=\"news-contents\">" << news[0].getContents() << "</div>"
             "</div>"
             "<a class=\"more\" href=\"" << news[0].url() << "#comments\">"
             "Comments (" << number(Comment::countForNews(news[0].id())) << ")</a>";
        if(news.size()>1){
            o << "<div class=\"oldnews\">"
              << "<h3>" << icon("newspaper-24") << " Older news</h3>"
              << "<ul>";
            for(std::vector<News>::const_iterator i=news.begin()+1; i!=news.end(); i++)
                o << "<li><a href=\"" << i->url() << "\">" << i->title() << "</a></li>";
            o << "</ul>"
              << "</div>";
        }
    }
    footer();
}
