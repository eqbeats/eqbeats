#include "FCgiIO.h"
#include "routing.h"
#include "session.h"
#include "timer.h"
#include "render/html.h"
#include "render/json.h"
#include "render/oembed.h"
#include "render/feed.h"
#include "render/http.h"
#include "actions/actions.h"
#include "account.h"
#include "utils.h"
#include <time.h>
#include <stdio.h>

#include "track.h"

using namespace cgicc;
using namespace Html;

std::string path;
cgicc::Cgicc *Action::cgi;

int main(int argc, char** argv){
    DB::connect();
    srand(getpid());

    if(!getenv("EQBEATS_DIR")){
        std::cerr << "Environment variable EQBEATS_DIR isn't set." << std::endl;
        return 1;
    }

    std::string log = eqbeatsDir()+"/eqbeats.log";
    freopen(log.c_str(),"a",stderr);

    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while(FCGX_Accept_r(&request) == 0){

        resetTimer();
        FCgiIO io(request);
        Cgicc cgi(&io);

        // Env
        Action::cgi = &cgi;
        serverName = cgi.getEnvironment().getServerName();

        // Routing
        int id;
        path = stripSlash(cgi.getEnvironment().getPathInfo());

        // Static
        if((id = routeAction("track", "vorbis", path)))
            io << Html::downloadTrack(id, Track::Vorbis);
        else if((id = routeAction("track", "mp3", path)))
            io << Html::downloadTrack(id, Track::MP3);
        else if((id = routeAction("track", "art", path)))
            io << Html::trackArt(id);
        else if((id = routeAction("track", "art/medium", path)))
            io << Html::trackArt(id, Art::Medium);
        else if((id = routeAction("track", "art/thumb", path)))
            io << Html::trackArt(id, Art::Thumbnail);
        // Json
        else if((id = routeAction("track", "json", path)))
            io << Json::track(id);
        else if((id = routeAction("user", "json", path)))
            io << Json::artist(id);
        else if(path == "/tracks/search/json")
            io << Json::tracks(Track::search(cgi("q")));
        else if(path == "/tracks/search/exact/json")
            io << Json::tracks(Track::exactSearch(cgi("artist"),cgi("track")));
        else if(path == "/tracks/latest/json")
            io << Json::tracks(Track::latest(50));
        else if(path == "/tracks/random/json")
            io << Json::tracks(Track::random(50));
        else if(path == "/tracks/popular/json")
            io << Json::tracks(Track::popular(50));
        else if(path == "/artists/json")
            io << Json::users(User::listArtists(200));
        else if(path == "/users/search/json")
            io << Json::users(User::search(cgi("q")));
        else if((id = routeAction("cat", "json", path)))
            io << Json::category(id);
        // Feeds
        else if((id = routeAction("cat", "atom", path)))
            io << Feed::category(id);
        else if(path == "/tracks/latest/atom")
            io << Feed::latest(200);
        else if((id = routeAction("user", "atom", path)))
            io << Feed::user(id);
        // oEmbed
        else if(path == "/oembed")
            io << oEmbed(cgi("url"), cgi("format")=="xml", number(cgi("maxwidth")));

        else{
        Session::start();
        // User
        if((id = routeId("user", path)))
            io << Html::userPage(id);
        else if((id = routeAction("user", "comment", path)))
            io << Action::postComment(Comment::User, id);
        else if((id = routeAction("user", "follow", path)))
            io << Action::follow(id, true);
        else if((id = routeAction("user", "unfollow", path)))
            io << Action::follow(id, false);
        else if((id = routeAction("user", "favorites", path)))
            io << Html::favorites(id);
        // Track
        else if((id = routeAction("track", "embed", path)))
            io << Html::embedTrack(id);
        else if((id = routeAction("track", "delete", path)))
            io << Action::deleteTrack(id);
        else if((id = routeAction("track", "rename", path)))
            io << Action::renameTrack(id);
        else if((id = routeAction("track", "notes", path)))
            io << Action::updateNotes(id);
        else if((id = routeAction("track", "upload", path)))
            io << Action::uploadTrack(id);
        else if((id = routeAction("track", "art/upload", path)))
            io << Action::uploadArt(id);
        else if((id = routeAction("track", "publish", path)))
            io << Action::publishTrack(id);
        else if((id = routeAction("track", "comment", path)))
            io << Action::postComment(Comment::Track, id);
        else if((id = routeAction("track", "cat", path)))
            io << Action::updateCategories(id);
        else if((id = routeAction("track", "favorite", path)))
            io << Action::favorite(id, true);
        else if((id = routeAction("track", "unfavorite", path)))
            io << Action::favorite(id, false);
        else if((id = routeId("track",path)))
            io << Html::trackPage(id);
        else if(path == "/track/new")
            io << Action::newTrack();
        // Tracks
        else if(path == "/tracks")
            io << Http::redirect("/");
        else if(path == "/tracks/search")
            io << Html::trackSearch(cgi("q"));
        else if(path == "/tracks/latest")
            io << Html::latestTracks(50);
        else if(path == "/tracks/random")
            io << Html::tracksPage("Random tracks", Track::random(50));
        else if(path == "/tracks/popular")
            io << Html::tracksPage("Popular tracks", Track::popular(50));
        // Categories
        else if((id = routeId("cat", path)))
            io << Html::category(id);
        // News
        else if((id = routeId("news", path)))
            io << Html::newsPage(id);
        else if((id = routeAction("news", "comment", path)))
            io << Action::postComment(Comment::News, id);
        else if(path == "/news")
            io << Html::latestNews(20);
        // Contests
        else if((id = routeId("contest", path)))
            io << Html::contest(id, cgi.getEnvironment().getRemoteAddr());
        else if((id = routeAction("contest", "submit", path)))
            io << Action::contestSubmission(id);
        else if((id = routeAction("contest", "vote", path)))
            io << Action::vote(id);
        // Users
        else if(path == "/users/search")
            io << Html::userSearch(cgi("q"));
        else if(path == "/users")
            io << Html::usersPage();
        else if(path == "/artists")
            io << Html::artistsPage();
        // Actions
        else if(path == "/register")
            io << Action::registration();
        else if(path == "/account")
            io << Action::account();
        else if(path == "/login")
            io << Action::login();
        else if(path == "/logout")
            io << Action::logout();
        // Static
        else if(path == "/quickstart")
            io << Html::quickStart();
        else if(path == "/faq")
            io << Html::faq();
        else if(path == "/credits")
            io << Html::credits();
        else if(path == "")
            io << Html::home();
        else
            io << Html::notFound();
        Session::destroy();
        }

        FCGX_Finish_r(&request);
    }

    DB::close();
    return 0;
}
