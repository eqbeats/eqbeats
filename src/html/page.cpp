#include "html.h"
#include "../timer.h"
#include "../session.h"
#include "../utils.h"
#include <stdio.h>
#include <sstream>

std::string statusMsg(int stat){
    switch(stat){
        case 200: return "200 OK";
        case 403: return "403 Forbidden";
        case 404: return "404 Not Found";
        case 500: return "500 Server Error";
    }
    return "";
}

std::string logStatus(){
    User u = Session::user();
    std::string redir = path=="/"||path.empty() ? "" : "?redirect=" + path;
    return u ?
        "Hi <b><a href=\"" + u.url() + "\">" + Html::escape(u.name()) + "</a></b>. <a href=\"/logout" + redir +"\">Logout</a>" :
        "<a href=\"/login" + redir + "\">Login</a>";
}

std::string Html::atomFeed(const std::string &url){
    return "<link href=\"" + url + "\" type=\"application/atom+xml\" rel=\"alternate\" />";
}

std::string Html::header(const std::string &title, const std::string &head, int status){
    return
        "Status: " + statusMsg(status) + "\n"
        "Content-Type: text/html; charset=UTF-8\n\n"
        "<!doctype html !>"
        "<html><head>"
            "<title>" + (title.empty()?"":title+" - ") + "Equestrian Beats</title>"
            "<link rel=\"stylesheet\" href=\"/static/style.css\" />"
            + head +
        "</head><body>"
            "<div id=\"main\">"
                "<div id=\"header\">"
                    "<h1><a href=\"/\">Equestrian Beats</a></h1>"
                    "<div id=\"logstatus\">"
                        + logStatus() +
                    "</div>"
                    "<div id=\"navbar\">"
                        "<a href=\"/\">Home</a> "
                        "<a href=\"/artists\">Artists</a> "
                        "<img src=\"/static/cm-nav.png\" /> "
                        "<a href=\"/news\">News</a> "
                        "<a href=\"/faq\">FAQ</a>"
                    "</div>"
                    "<div style=\"clear:both;\"></div>"
                "</div>"
                "<div id=\"contents\">";
}

std::string Html::footer(){
    return
            "</div>"
            "<div id=\"footer\">"
                "Contact: " + escapeEmail("contact@eqbeats.org") +
                //"Generated in " + number(usecs()) + " &mu;S."
            "</div>"
        "</div>"
      "</body></html>";
}

std::string Html::notFound(const std::string &what){
    return header(what + " not found", "", 404)
         + "<h2>" + what + " not found"
         + footer();
}

std::string Html::errorPage(const std::string &err){
    return header("Error")
        + "<div class=\"error\">" + err + "</div>"
        + footer();
}

std::string Html::quickStart(){
    return header("Thanks")
        + "<div class=\"message\">Now that you've registered you can upload tracks: just click your name at the top right corner of the page.</div>"
        + footer();
}
