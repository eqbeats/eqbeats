#include "page.h"
#include "escape.h"
#include "../render.h"
#include "../http.h"
#include "../../timer.h"
#include "../../path.h"
#include "../../user.h"
#include "../../session.h"
#include "../../number.h"
#include <stdio.h>

using namespace Render;

void logStatus(){
    User u = Session::user();
    std::string path = getPath();
    std::string redir = path=="/"||path.empty() ? "" : "?redirect=" + path;
    o << (u ?
        "Hi <b><a href=\"" + u.url() + "\">" + Html::escape(u.name()) + "</a></b>. <a href=\"/logout" + redir +"\">Logout</a>" :
        "<a href=\"/login" + redir + "\">Login</a>");
}

void Html::header(const std::string &title, const std::string &head, int status){
    Http::header("text/html", status);
    o << "<!DOCTYPE html>"
         "<html><head>"
            "<title>" << (title.empty()?"":escape(title)+" - ") << "Equestrian Beats</title>"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
            "<link rel=\"stylesheet\" type=\"text/css\" href=\"/static/style.css\" />"
            "<link rel=\"shortcut icon\" href=\"/static/favicon.ico\" id=\"favicon-link\" />"
            "<script type=\"text/javascript\" src=\"/static/sm2.js\"></script>"
            "<script type=\"text/javascript\" src=\"/static/player.js\"></script>"
            << head <<
        "</head><body>"
            "<div id=\"main\">"
                "<div id=\"header\">"
                    "<h1><a href=\"/\">Equestrian Beats</a></h1>"
                    "<div id=\"logstatus\">";
                         logStatus();
    o <<            "</div>"
                    "<div id=\"navbar\">"
                        "<a href=\"/\">Home</a> "
                        "<a href=\"/artists\">Artists</a> "
                        "<img src=\"/static/cm-nav.png\" alt=\"-\"/> "
                        "<a href=\"/news\">News</a> "
                        "<a href=\"/faq\">FAQ</a>"
                    "</div>"
                    "<div style=\"clear:both;\"></div>"
                "</div>"
                "<div id=\"contents\">";
}

void Html::footer(){
    o <<    "</div>"
            "<div id=\"footer\">"
                "Contact: " << escapeEmail("contact@eqbeats.org") <<
                "<br /><a href=\"/credits\">Credits</a>"
                "<!--Generated in " << number(usecs()) << " µS by " << number(getpid()) << ".-->"
            "</div>"
        "</div>"
      "<script>document.body.className = 'js';</script>"
      "</body></html>";
}

std::string Html::metaDescription(std::string descr){
    descr = escape(descr);
    if(descr.size() > 160){
        int pos = descr.rfind(' ', 160);
        descr = descr.substr(0, pos>160?160:pos) + "&hellip;";
    }
    return "<meta name=\"description\" content=\"" + descr + "\" />";
}

void Html::notFound(const std::string &what){
    header(what + " not found", "", 404);
    o << "<h2>" << what << " not found";
    footer();
}

void Html::errorPage(const std::string &err){
    header("Error");
    o << "<div class=\"error\">" << err << "</div>";
    footer();
}
