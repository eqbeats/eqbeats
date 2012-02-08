#include "../html.h"
#include "../http.h"
#include "../../timer.h"
#include "../../session.h"
#include "../../utils.h"
#include <stdio.h>
#include <sstream>

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
        Http::header("text/html", status) +
        "<html><head>"
            "<title>" + (title.empty()?"":escape(title)+" - ") + "Equestrian Beats</title>"
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
                "<br /><a href=\"/credits\">Credits</a>"
                "<!--Generated in " + number(usecs()) + " μS.-->"
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
