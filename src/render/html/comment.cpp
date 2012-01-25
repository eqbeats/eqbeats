#include "../html.h"
#include "../../session.h"
#include <sstream>

using namespace std;

string Html::comments(const vector<Comment> &cmts, const std::string title){
    stringstream s;
    s << "<h3 id=\"comments\"><img src=\"/static/balloon.png\" /> " << title << "</h3><div>";
    if(cmts.empty())
        s << "<div class=\"empty\">No comments yet.</div>";
    for(vector<Comment>::const_iterator i=cmts.begin(); i!=cmts.end(); i++){
            s << "<div class=\"comment\">" << Html::format(i->contents) << "<br />"
                 "<div class=\"by\">by ";
            if(i->authorId)
            s << "<a href=\""<< User::url(i->authorId) << "\">" << Html::escape(i->authorName()) << "</a>";
        else
            s << Html::escape(i->authorName());
        if(i->trackId)
            s << " on <a href=\"" << Track::url(i->trackId) << "\">" << Html::escape(i->trackTitle) << "</a>";
        s << "</div></div>";
    }
    s << "</div>";
    return s.str();
}

string Html::commentForm(const string &action){
    return
        "<form class=\"postcomment\" action=\"" + action + "\" method=\"post\">" 
         "<h3>Post a comment</h3>"
         + (Session::user() ? "" : "Name : <input type=\"text\" name=\"name\" /><br />")
         + "<textarea name=\"msg\"></textarea><br />"
            "<input type=\"submit\" value=\"Post a comment\" onclick=\"this.form.submit();this.disabled=true;return false;\" />"
         "</form>";
}
