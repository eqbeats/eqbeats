#include "session.h"
#include "timer.h"
#include "render/json.h"
#include "render/oembed.h"
#include "render/feed.h"
#include "render/download.h"
#include "render/http.h"
#include "render/html/track.h"
#include "render/html/player.h"
#include "render/html/user.h"
#include "render/html/page.h"
#include "render/html/news.h"
#include "render/html/static.h"
#include "render/html/home.h"
#include "render/html/contest.h"
#include "actions/actions.h"
#include "account.h"
#include "path.h"
#include "db.h"
#include "track.h"
#include "number.h"
#include "render/render.h"
#include "cgi.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>

using namespace cgicc;
using namespace Render;
using namespace Action;

Cgicc cgi;

int main(int argc, char** argv){
    DB::connect();
    srand(getpid());

    if(!getenv("EQBEATS_DIR")){
        std::cerr << "Environment variable EQBEATS_DIR isn't set." << std::endl;
        return 1;
    }

    std::string logfile = eqbeatsDir()+"/eqbeats.log";
    freopen(logfile.c_str(),"a",stderr);

    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    std::string path;
    int id;
    while(FCGX_Accept_r(&request) == 0){
        resetTimer();
        o.attach(&request);
        cgi = Cgicc(&o);
        path = getPath();

        // Static
        if((id = route("track", "vorbis", path)))
            Http::downloadTrack(id, Track::Vorbis);
        else if((id = route("track", "mp3", path)))
            Http::downloadTrack(id, Track::MP3);
        else if((id = route("track", "art", path)))
            Http::trackArt(id);
        else if((id = route("track", "art/medium", path)))
            Http::trackArt(id, Art::Medium);
        else if((id = route("track", "art/thumb", path)))
            Http::trackArt(id, Art::Thumbnail);
        // Json
        else if((id = route("track", "json", path)))
            Json::track(id);
        else if((id = route("user", "json", path)))
            Json::artist(id);
        else if(path == "/tracks/search/json")
            Json::tracks(Track::search(cgi("q")));
        else if(path == "/tracks/search/exact/json")
            Json::tracks(Track::exactSearch(cgi("artist"),cgi("track")));
        else if(path == "/tracks/latest/json")
            Json::tracks(Track::latest(50));
        else if(path == "/tracks/random/json")
            Json::tracks(Track::random(50));
        else if(path == "/tracks/featured/json")
            Json::tracks(Track::featured(50));
        else if(path == "/artists/json")
            Json::users(User::listArtists(200));
        else if(path == "/users/search/json")
            Json::users(User::search(cgi("q")));
        // Feeds
        else if(path == "/tracks/latest/atom")
            Feed::latest(200);
        else if(path == "/tracks/featured/atom")
            Feed::featured(50);
        else if((id = route("user", "atom", path)))
            Feed::user(id);
        // oEmbed
        else if(path == "/oembed")
            oEmbed(cgi("url"), cgi("format")=="xml", number(cgi("maxwidth")));

        else{
        Session::start();
        // User
        if((id = route("user", path)))
            Html::userPage(id);
        else if((id = route("user", "comment", path)))
            Action::postComment(Comment::User, id);
        else if((id = route("user", "follow", path)))
            Action::follow(id, true);
        else if((id = route("user", "unfollow", path)))
            Action::follow(id, false);
        else if((id = route("user", "favorites", path)))
            Html::favorites(id);
        // Track
        else if((id = route("track", "embed", path)))
            Html::embedTrack(id);
        else if((id = route("track", "delete", path)))
            Action::deleteTrack(id);
        else if((id = route("track", "rename", path)))
            Action::renameTrack(id);
        else if((id = route("track", "notes", path)))
            Action::updateNotes(id);
        else if((id = route("track", "upload", path)))
            Action::uploadTrack(id);
        else if((id = route("track", "art/upload", path)))
            Action::uploadArt(id);
        else if((id = route("track", "publish", path)))
            Action::publishTrack(id);
        else if((id = route("track", "comment", path)))
            Action::postComment(Comment::Track, id);
        else if((id = route("track", "favorite", path)))
            Action::favorite(id, true);
        else if((id = route("track", "unfavorite", path)))
            Action::favorite(id, false);
        else if((id = route("track", "report", path)))
            Action::reportTrack(id);
        else if((id = route("track", "flags", path)))
            Action::setFlags(id);
        else if((id = route("track", "tags", path)))
            Action::setTags(id);
        else if((id = route("track",path)))
            Html::trackPage(id);
        else if(path == "/track/new")
            Action::newTrack();
        // Tracks
        else if(path == "/tracks")
            Http::redirect("/");
        else if(path == "/tracks/search")
            Html::trackSearch(cgi("q"));
        else if(path == "/tracks/latest")
            Html::latestTracks(15);
        else if(path == "/tracks/random")
            Html::tracksPage("Random tracks", Track::random(15));
        else if(path == "/tracks/featured")
            Html::tracksPage("Featured tracks", Track::featured(15));
        else if(path.substr(0,12) == "/tracks/tag/"){
            std::string tag = path.substr(12);
            Html::tracksPage(tag, Track::byTag(tag));
        }
        // News
        else if((id = route("news", path)))
            Html::newsPage(id);
        else if((id = route("news", "comment", path)))
            Action::postComment(Comment::News, id);
        else if(path == "/news")
            Html::latestNews(20);
        // Contests
        else if((id = route("contest", path)))
            Html::contest(id);
        else if((id = route("contest", "submit", path)))
            Action::contestSubmission(id);
        else if((id = route("contest", "vote", path)))
            Action::vote(id);
        // Users
        else if(path == "/users/search")
            Html::userSearch(cgi("q"));
        else if(path == "/artists")
            Html::artistsPage();
        // Actions
        else if(path == "/register")
            Action::registration();
        else if(path == "/account")
            Action::account();
        else if(path == "/account/reset")
            Action::passwordReset();
        else if(path == "/login")
            Action::login();
        else if(path == "/logout")
            Action::logout();
        // Static
        else if(path == "/quickstart")
            Html::quickStart();
        else if(path == "/faq")
            Html::faq();
        else if(path == "/credits")
            Html::credits();
        else if(path == "")
            Html::home();
        else
            Html::notFound();
        Session::destroy();
        }

        FCGX_Finish_r(&request);
    }

    DB::close();
    return 0;
}
