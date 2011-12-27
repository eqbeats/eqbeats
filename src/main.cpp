#include "FCgiIO.h"
#include "routing.h"
#include "session.h"
#include "timer.h"
#include "html/html.h"
#include "actions/actions.h"
#include "account.h"
#include <time.h>

using namespace cgicc;
using namespace Html;

std::string path;

int main(int argc, char** argv){
    DB::connect();
    srand(time(NULL));

    //freopen("/srv/eqbeats/eqbeats.log","a",stdout);
    freopen("/srv/eqbeats/eqbeats.log","a",stderr);

    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while(FCGX_Accept_r(&request) == 0){

        resetTimer();
        FCgiIO io(request);
        Cgicc cgi(&io);
        Session::start(cgi);

        //visit(cgi.getEnvironment().getRemoteAddr());

        // Routing
        int id;
        path = cgi.getEnvironment().getPathInfo();
        if((id = routeUser(path)))
            io << Html::userPage(id);
        else if((id = routeTrackAction("delete", path)))
            io << Action::deleteTrack(id, cgi);
        else if((id = routeTrackAction("rename", path)))
            io << Action::renameTrack(id, cgi);
        else if((id = routeTrackAction("notes", path)))
            io << Action::updateNotes(id, cgi);
        else if((id = routeTrackAction("upload", path)))
            io << Action::uploadTrack(id, cgi);
        else if((id = routeTrackAction("flac", path)))
            io << Html::downloadTrack(id, Track::FLAC);
        else if((id = routeTrackAction("vorbis", path)))
            io << Html::downloadTrack(id, Track::Vorbis);
        else if((id = routeTrackAction("mp3", path)))
            io << Html::downloadTrack(id, Track::MP3);
        else if((id = routeTrackAction("visibility", path)))
            io << Action::trackVisibility(id, cgi);
        else if((id = routeTrackAction("comment", path)))
            io << Action::postComment(id, cgi);
        else if((id = routeTrack(path)))
            io << Html::trackPage(id);
        else if(path == "/track/new")
            io << Action::newTrack(cgi);
        else if(path == "/tracks")
            io << Html::tracksPage();
        else if(path == "/tracks/search")
            io << Html::trackSearch(cgi("q"));
        else if(path == "/tracks/latest")
            io << Html::latestTracks(50);
        else if(path == "/tracks/random")
            io << Html::randomTracks(50);
        else if(path == "/tracks/popular")
            io << Html::popularTracks(50);
        else if((id = routeNews(path)))
            io << Html::newsPage(id);
        else if(path == "/users/search")
            io << Html::userSearch(cgi("q"));
        else if(path == "/users")
            io << Html::usersPage();
        else if(path == "/artists")
            io << Html::artistsPage();
        else if(path == "/register")
            io << Action::registration(cgi);
        else if(path == "/account")
            io << Action::account(cgi);
        else if(path == "/login")
            io << Action::login(cgi);
        else if(path == "/logout")
            io << Action::logout(cgi);
        else if(path == "/")
            io << Html::latestNews(20);
        else
            io << Html::notFound();

        Session::destroy();
        FCGX_Finish_r(&request);
    }

    DB::close();
    return 0;
}
