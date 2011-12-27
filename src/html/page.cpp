#include "html.h"
#include "../timer.h"
#include "../session.h"
#include "../utils.h"
#include <stdio.h>
#include <sstream>

std::string statusMsg(int stat){
    switch(stat){
        case 200: return "200 OK";
        case 404: return "404 Not Found";
        case 500: return "500 Server Error";
    }
    return "";
}

std::string logStatus(){
    User u = Session::user();
    return u ?
        "Hi <b><a href=\"" + u.url() + "\">" + u.name() + "</a></b>. <a href=\"/logout?redirect="+ path +"\">Logout</a>" :
        "<a href=\"/login?redirect=" + path + "\">Login</a>";
}

std::string Html::header(const std::string &title, int status){
    return
        "Status: " + statusMsg(status) + "\n"
        "Content-Type: text/html; charset=UTF-8\n\n"
        "<!doctype html !>"
        "<html><head>"
            "<title>" + (title.empty()?"":title+" - ") + "Equestrian Beats</title>"
            "<link rel=\"stylesheet\" href=\"/static/style.css\" />"
        "</head><body>"
            "<div id=\"header\">"
                "<h1><a href=\"/\">Equestrian Beats</a></h1>"
                "<div id=\"logstatus\">"
                    + logStatus() +
                "</div>"
                "<ul id=\"navbar\">"
                    "<li><a href=\"/tracks\">Tracks:</a></li>"
                    "<li><a href=\"/tracks/latest\">Latest</a></li>"
                    "<li><a href=\"/tracks/popular\">Popular</a></li>"
                    "<li><a href=\"/tracks/random\">Random</a></li>"
                    "<li><img src=\"/static/cm-nav.png\" /></li>"
                    "<li><a href=\"/artists\">Artists</a></li>"
                "</ul>"
                "<div style=\"clear:both;\"></div>"
            "</div>"
            "<div id=\"contents\">" +
                (title.empty()?"":"<h2>" + title + "</h2>");
}

std::string Html::footer(){
    return
        "</div>"
        "<div id=\"footer\">"
            "Contact: " + escapeEmail("contact@eqbeats.org") +
            //"Generated in " + number(usecs()) + " &mu;S."
        "</div>"
      "</body></html>";
}

std::string Html::notFound(){
    return header("Not found", 404) + footer();
}

std::string Html::errorPage(const std::string &err){
    return header("Error")
        + "<div class=\"error\">" + err + "</div>"
        + footer();
}
