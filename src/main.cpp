#include "FCgiIO.h"
#include "routing.h"
#include "session.h"
#include "timer.h"
#include "html/html.h"
#include "actions/actions.h"
#include "account.h"
#include "utils.h"
#include <time.h>

#include "track.h"

using namespace cgicc;
using namespace Html;

std::string path;

int main(int argc, char** argv){
    DB::connect();
    srand(time(NULL));

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
        serverName = cgi.getEnvironment().getServerName();
        Session::start(cgi);

        // Routing
        int id;
        path = stripSlash(cgi.getEnvironment().getPathInfo());

        // User
        if((id = routeId("user", path)))
            io << Html::userPage(id);
        else if((id = routeAction("user", "comment", path)))
            io << Action::postComment(Comment::User, id, cgi);
        else if((id = routeAction("user", "follow", path)))
            io << Action::follow(id, true, cgi);
        else if((id = routeAction("user", "unfollow", path)))
            io << Action::follow(id, false, cgi);
        else if((id = routeAction("user", "favorites", path)))
            io << Html::favorites(id);
        // Track
        else if((id = routeAction("track", "embed", path)))
            io << Html::embedTrack(id);
        else if((id = routeAction("track", "delete", path)))
            io << Action::deleteTrack(id, cgi);
        else if((id = routeAction("track", "rename", path)))
            io << Action::renameTrack(id, cgi);
        else if((id = routeAction("track", "notes", path)))
            io << Action::updateNotes(id, cgi);
        else if((id = routeAction("track", "upload", path)))
            io << Action::uploadTrack(id, cgi);
        else if((id = routeAction("track", "vorbis", path)))
            io << Html::downloadTrack(id, Track::Vorbis);
        else if((id = routeAction("track", "mp3", path)))
            io << Html::downloadTrack(id, Track::MP3);
        else if((id = routeAction("track", "art", path)))
            io << Html::trackArt(id);
        else if((id = routeAction("track", "art/upload", path)))
            io << Action::uploadArt(id, cgi);
        else if((id = routeAction("track", "publish", path)))
            io << Action::publishTrack(id, cgi);
        else if((id = routeAction("track", "comment", path)))
            io << Action::postComment(Comment::Track, id, cgi);
        else if((id = routeAction("track", "cat", path)))
            io << Action::updateCategories(id, cgi);
        else if((id = routeAction("track", "favorite", path)))
            io << Action::favorite(id, true, cgi);
        else if((id = routeAction("track", "unfavorite", path)))
            io << Action::favorite(id, false, cgi);
        else if((id = routeId("track",path)))
            io << Html::trackPage(id);
        else if(path == "/track/new")
            io << Action::newTrack(cgi);
        // Tracks
        else if(path == "/tracks")
            io << Html::redirect("/");
        else if(path == "/tracks/search")
            io << Html::trackSearch(cgi("q"));
        else if(path == "/tracks/latest")
            io << Html::latestTracks(50);
        else if(path == "/tracks/random")
            io << Html::tracksPage("Random tracks", Track::random(50));
        else if(path == "/tracks/popular")
            io << Html::tracksPage("Popular tracks", Track::popular(50));
        // Feeds
        else if(path == "/tracks/latest/atom")
            io << Html::tracksFeed(200);
        else if((id = routeAction("user", "atom", path)))
            io << Html::userFeed(id);
        else if((id = routeAction("cat", "atom", path)))
            io << Html::categoryFeed(id);
        else if((id = routeId("cat", path)))
            io << Html::category(id);
        // oEmbed
        else if(path == "/oembed")
            io << Html::oEmbed(cgi("url"), cgi("format")=="xml", number(cgi("maxwidth")));
        // News
        else if((id = routeId("news", path)))
            io << Html::newsPage(id);
        else if((id = routeAction("news", "comment", path)))
            io << Action::postComment(Comment::News, id, cgi);
        else if(path == "/news")
            io << Html::latestNews(20);
        // Users
        else if(path == "/users/search")
            io << Html::userSearch(cgi("q"));
        else if(path == "/users")
            io << Html::usersPage();
        else if(path == "/artists")
            io << Html::artistsPage();
        // Actions
        else if(path == "/register")
            io << Action::registration(cgi);
        else if(path == "/account")
            io << Action::account(cgi);
        else if(path == "/login")
            io << Action::login(cgi);
        else if(path == "/logout")
            io << Action::logout(cgi);
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
        FCGX_Finish_r(&request);
    }

    DB::close();
    return 0;
}
