#include "html.h"
#include "../session.h"
#include <sstream>

using namespace std;

string Html::comments(const vector<Comment> &cmts, const std::string title){
    stringstream s;
    s << "<h3 id=\"comments\">" << title << "</h3><div>";
    for(vector<Comment>::const_iterator i=cmts.begin(); i!=cmts.end(); i++){
        s << "<div class=\"comment\">" << Html::format(i->contents) << "<br />"
             "<div class=\"by\">by ";
        if(i->authorId)
            s << "<a href=\""<< User::url(i->authorId) << "\">" << i->authorName() << "</a>";
        else
            s << i->authorName();
        if(i->trackId)
            s << " on <a href=\"" << Track::url(i->trackId) << "\">" << i->trackTitle << "</a>";
        s << "</div></div>";
    }
    s << "</div>";
    return s.str();
}

string Html::commentForm(const string &action){
    return
        "<form action=\"" + action + "\" method=\"post\">" 
         + (Session::user() ? "" : "Name : <input type=\"text\" name=\"name\" /><br />")
         + "<textarea name=\"msg\"></textarea><br />"
            "<input type=\"submit\" value=\"Post\" />"
         "</form>";
}
