#include "html.h"
#include "../session.h"
#include "../utils.h"
#include "../account.h"
#include <sstream>

using namespace std;

string comments(int uid){
    vector<Track::Comment> cmts = Track::Comment::forArtist(uid);
    if(cmts.empty()) return std::string();
    stringstream s;
    s << "<h3>Comments</h3><div>";
    for(vector<Track::Comment>::const_iterator i=cmts.begin(); i!=cmts.end(); i++){
        s << "<div class=\"comment\">"
          << Html::format(i->contents()) << "<br />"
             "<div class=\"by\">by ";
        if(i->authorId())
            s << "<a href=\""<< User::url(i->authorId()) << "\">" << i->authorName() << "</a>";
        else
            s << i->authorName();
        s << " on <a href=\"" << Track::url(i->trackId()) << "\">" << i->trackTitle() << "</a></div></div>";
    }
    s << "</div>";
    return s.str();
}

string Html::userPage(int uid){
    Account user(uid);
    if(!user)
        return header("User not found", 404) + footer();
    stringstream s;
    s << header(escape(user.name()));
    s << "<div class=\"user\">"
             "Email: " << escapeEmail(user.email())
             << "<div class=\"notes\">" << format(user.about()) << "</div>";
    bool edition = Session::user().id() == user.id();
    if(edition)
        s << "<a href=\"/account\"><b>Edit</b></a>";
    s << "</div>";
    vector<Track> tracks = Track::byArtist(user.id(), edition);
    if(!tracks.empty())
        s << "<h3>Tracks</h3>"
          << Html::trackList(tracks, edition ? Html::Edition : Html::Compact);
    if(edition)
        s << uploadForm("/track/new") << comments(uid);
    s << footer();
    return s.str();
}

string Html::userList(const vector<User> &users){
    stringstream s(stringstream::out);
    s << "<ul>";
    for(vector<User>::const_iterator i=users.begin(); i!=users.end(); i++)
        s << "<li><a href=\"" << i->url() << "\">" << escape(i->name()) << "</a></li>";
    s << "</ul>";
    return s.str();
}

string Html::usersPage(){
    return header("Users")
         + searchForm("/users/search")
         + userList(User::list(50))
         + footer();
}

string Html::artistsPage(){
    return header("Artists")
         + searchForm("/users/search")
         + userList(User::listArtists(50))
         + footer();
}

string Html::searchForm(const string &action, const string &q){
    return
        "<form action=\"" + action + "\">"
            "<input type=\"text\" name=\"q\" value=\"" + escape(q) + "\" />"
            "<input type=\"submit\" value=\"Search\" />"
        "</form>";
}

string Html::userSearch(const std::string &q){
    vector<User> res = User::search(q);
    stringstream s;
    s << header("User search")
      << searchForm("/users/search", q);
    if(res.empty())
        s << "No result.";
    else
        s << userList(res);
    s << footer();
    return s.str();
}
