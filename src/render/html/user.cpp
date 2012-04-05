#include "page.h"
#include "escape.h"
#include "feed.h"
#include "comment.h"
#include "forms.h"
#include "track.h"
#include "user.h"
#include "event.h"
#include "../render.h"
#include "../../session.h"
#include "../../track.h"
#include "../../comment.h"
#include "../../number.h"
#include "../../account.h"
#include "../../follower.h"
#include "../../event.h"

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
    string about = user.about();

    header(user.name(), (about.empty()?"":metaDescription(strip(about))) + atomFeed(user.url() + "/atom"));

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
             "<div class=\"item\">" << icon("mail") << " Email: " << escapeEmail(user.email()) << "</div>";
    bool edition = Session::user().id() == user.id();
    if(edition)
        o << "<div class=\"item\">" << icon("card-pencil") << " <a href=\"/account\">Edit</a></div>";
    o << "<div class=\"item\">" << icon("star") << " <a href=\"" << user.url() << "/favorites\">Favorite tracks</a></div>";
    if(!about.empty())
        o << "<div class=\"notes\">" << format(about) << "</div>";
    o << "</div>";

    // Feed
    o << "<div class=\"events_wrapper\">";
    Html::commentForm(user.url()+"/comment", true);
    Html::eventStream(Event::userEvents(user, 8), "Recent happenings");
    o << "</div>";

    // Tracks
    vector<Track> tracks = user.tracks(edition);
    if(edition || !tracks.empty()){
        o << "<h3>" << icon("disc") << " Tracks ";
        feedIcon(user.url() + "/atom");
        o << "</h3>";
        Html::trackList(tracks);
    }

    // Edition
    if(edition){
        uploadForm("/track/new");
        o << "<h3>" << icon("plus-circle") << " Artists you follow</h3>";
        userList(Follower(user).following());
    }

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
