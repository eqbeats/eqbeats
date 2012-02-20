#include "../html.h"
#include "../../session.h"
#include "../../number.h"
#include "../../account.h"

using namespace std;
using namespace Render;

void followPlus(const User &u){
    if(Session::user() == u) return;
    bool isFollowed = Session::user().isFollowing(u.id());
    Render::o << "<a class=\"follow" << (isFollowed?"":" disabled")
              << "\" href=\"" << (Session::user() ? "" : "/login?redirect=")
              << u.url() << "/" << (isFollowed?"un":"") << "follow\">"
              << (isFollowed?"Stop following":"Follow")
              << "</a>";
}

void Html::userPage(int uid){
    Account user(uid);
    if(!user) return notFound("User");
    header(user.name(), atomFeed(user.url() + "/atom"));
    o << "<h2>" << escape(user.name()) << " ";
    followPlus(user);
    o << "</h2>"
         "<div class=\"user\">"
             "<div class=\"item\"><img src=\"/static/mail.png\" /> Email: " << escapeEmail(user.email()) << "</div>";
    string about = user.about();
    if(!about.empty())
        o << "<div class=\"notes\">" << format(about) << "</div>";
    bool edition = Session::user().id() == user.id();
    if(edition)
        o << "<div class=\"item\"><img src=\"/static/card-pencil.png\" /> <a href=\"/account\">Edit</a></div>";
    o << "<div class=\"item\"><img src=\"/static/star.png\" /> <a href=\"" << user.url() << "/favorites\">Favorite tracks</a></div>"
         "</div>";
    vector<Track> tracks = Track::byArtist(user.id(), edition);
    if(edition || !tracks.empty())
        o << "<h3><img src=\"/static/disc.png\" /> Tracks ";
        feedIcon(user.url() + "/atom");
        o << "</h3>";
        Html::trackList(tracks, Html::Compact);
    if(edition){
        uploadForm("/track/new");
        o << "<h3><img src=\"/static/plus-circle.png\" /> Artists you follow</h3>";
        Html::userList(user.following());
        if(!tracks.empty())
            Html::comments(Comment::forArtist(uid), "Comments on your tracks");
    }
    Html::comments(Comment::forUser(uid));
    Html::commentForm(user.url()+"/comment");;
    footer();
}

void Html::favorites(int uid){
    User u(uid);
    if(!u) return notFound("User");
    Html::tracksPage(Html::escape(u.name())+" - Favorite tracks", Track::favorites(uid));
}

void Html::userList(const vector<User> &users){
    if(users.empty()) o << "<div class=\"empty\">Nobody here yet.</div>";
    o << "<ul>";
    for(vector<User>::const_iterator i=users.begin(); i!=users.end(); i++)
        o << "<li><a href=\"" << i->url() << "\">" << escape(i->name()) << "</a></li>";
    o << "</ul>";
}

void Html::usersPage(){
    header("Users");
    o << "<h2>Users</h2>";
    searchForm("/users/search");
    userList(User::list(50));
    footer();
}

void Html::artistsPage(){
    header("Artists");
    o << "<h2>Artists</h2>";
    searchForm("/users/search");
    userList(User::listArtists(200));
    footer();
}

void Html::searchForm(const string &action, const string &q){
    o << "<form action=\"" << action << "\">"
            "<input type=\"text\" name=\"q\" value=\"" << escape(q) << "\" />"
            "<input type=\"submit\" value=\"Search\" />"
        "</form>";
}

void Html::userSearch(const std::string &q){
    vector<User> res = User::search(q);
    header("User search");
    o << "<h2>User search</h2>";
    searchForm("/users/search", q);
    if(res.empty())
        o << "No result.";
    else
        userList(res);
    footer();
}
