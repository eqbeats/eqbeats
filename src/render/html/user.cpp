#include "page.h"
#include "escape.h"
#include "feed.h"
#include "comment.h"
#include "forms.h"
#include "track.h"
#include "user.h"
#include "../render.h"
#include "../../session.h"
#include "../../track.h"
#include "../../comment.h"
#include "../../number.h"
#include "../../account.h"
#include "../../follower.h"

using namespace std;
using namespace Render;

void userList(const vector<User> &users){
    if(users.empty()) o << "<div class=\"empty\">Nobody here yet.</div>";
    o << "<ul>";
    for(vector<User>::const_iterator i=users.begin(); i!=users.end(); i++)
        o << "<li><a href=\"" << i->url() << "\">" << Html::escape(i->name()) << "</a></li>";
    o << "</ul>";
}

void Html::userPage(int uid){
    Account user(uid);
    if(!user) return notFound("User");
    header(user.name(), atomFeed(user.url() + "/atom"));

    // Title
    o << "<h2>" << escape(user.name()) << " ";
    if(Session::user() != user){
        bool isFollowed = Follower(Session::user()).isFollowing(user.id());
        Render::o << "<a class=\"follow" << (isFollowed?"":" disabled")
                  << "\" href=\"" << (Session::user() ? "" : "/login?redirect=")
                  << user.url() << "/" << (isFollowed?"un":"") << "follow\">"
                     "<span>" << (isFollowed?"Stop following":"Follow") << "</span>"
                     "</a>";
    }
    o << "</h2>"

    // Description
         "<div class=\"user\">"
             "<div class=\"item\"><img src=\"/static/mail.png\" alt=\"\" /> Email: " << escapeEmail(user.email()) << "</div>";
    string about = user.about();
    if(!about.empty())
        o << "<div class=\"notes\">" << format(about) << "</div>";
    bool edition = Session::user().id() == user.id();
    if(edition)
        o << "<div class=\"item\"><img src=\"/static/card-pencil.png\" alt=\"\" /> <a href=\"/account\">Edit</a></div>";
    o << "<div class=\"item\"><img src=\"/static/star.png\" alt=\"\" /> <a href=\"" << user.url() << "/favorites\">Favorite tracks</a></div>"
         "</div>";

    // Tracks
    vector<Track> tracks = user.tracks(edition);
    if(edition || !tracks.empty()){
        o << "<h3><img src=\"/static/disc.png\" alt=\"\" /> Tracks ";
        feedIcon(user.url() + "/atom");
        o << "</h3>";
        Html::trackList(tracks, Html::Compact);
    }

    // Edition
    if(edition){
        uploadForm("/track/new");
        o << "<h3><img src=\"/static/plus-circle.png\" alt=\"\" /> Artists you follow</h3>";
        userList(Follower(user).following());
        if(!tracks.empty())
            Html::comments(Comment::forArtist(uid), "Comments on your tracks");
    }

    // Comments
    Html::comments(Comment::forUser(uid));
    Html::commentForm(user.url()+"/comment");;

    footer();
}

void Html::favorites(int uid){
    User u(uid);
    if(!u) return notFound("User");
    Html::tracksPage(Html::escape(u.name())+" - Favorite tracks", Track::favorites(uid));
}

void Html::artistsPage(){
    header("Artists");
    o << "<h2>Artists</h2>";
    searchForm("/users/search");
    userList(User::listArtists(200));
    footer();
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
