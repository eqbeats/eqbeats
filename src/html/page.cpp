#include "html.h"
#include "../timer.h"
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

std::string Html::header(std::string title, int status){
    return header(title, std::string(), status);
}

std::string Html::header(std::string title, const std::string &head, int status){
    if(!title.empty()) title+= " - ";
    return
        "Status: " + statusMsg(status) + "\n"
        "Content-Type: text/html; charset=UTF-8\n\n"
        "<!doctype html !>"
        "<html><head>"
            "<title>" + title + "Equestrian Beats</title>"
            "<link rel=\"stylesheet\" href=\"/static/style.css\" />"
            + head +
        "</head><body>"
            "<div id=\"header\"><a href=\"/\">Equestrian Beats</a></div>";
}

std::string Html::footer(){
    std::stringstream s(std::stringstream::out);
    s << "<div id=\"footer\">Generated in " << usecs() << " &mu;S.</div>"
         "</body></html>";
    return s.str();
}

std::string Html::notFound(){
    return header("Not found", 404) +
        "<h2>404 Not found</h2>"
        "<p>The page you requested couldn't be found.</p>"
        + footer();
}
