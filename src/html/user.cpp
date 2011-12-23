#include "html.h"
#include "../session.h"
#include <sstream>

using namespace std;

string Html::userPage(const User &user, const string &msg){
    if(!user)
        return header("User not found", 404) + "User not found" + footer();
    stringstream s;
    s << header(escape(user.name()));
    if(msg == "trackdeleted")
        s << "<div class=\"message\">Track successfuly deleted.</div>";
    s << "<div class=\"user\">"
         "<h3>" << escape(user.name()) << "</h3>";
    bool edition = Session::user() == user;
    vector<Track> tracks = Track::byArtist(user.id(), edition);
    if(!tracks.empty())
        s << "<h4>Tracks</h4>"
          << Html::trackList(tracks, edition ? Html::Edition : Html::Compact);
    if(edition)
        s << "<form method=\"post\" action=\"/track/new\">"
                "<input name=\"title\" />"
                "<input type=\"submit\" value=\"New track\" />"
             "</form>";
    s << "</div>" << footer();
    return s.str();
}

string Html::userList(const vector<User> &users){
    stringstream s(stringstream::out);
    s << "<ul>";
    for(vector<User>::const_iterator i=users.begin(); i!=users.end(); i++)
        s << "<li><a href=\"" << i->pageUrl() << "\">" << escape(i->name()) << "</a></li>";
    s << "</ul>";
    return s.str();
}

string Html::usersPage(){
    return header("Users") + userList(User::list(20)) + footer();
}

string Html::artistsPage(){
    return header("Artists") + userList(User::listArtists(20)) + footer();
}
