#include <iostream>
#include <cgicc/Cgicc.h>
#include "FCgiIO.h"
#include "db.h"
#include "user.h"
#include "track.h"
#include "routing.h"
#include "session.h"
#include "timer.h"
#include "html/html.h"
#include "actions/actions.h"
#include "account.h"
#include <time.h>

using namespace cgicc;
using namespace Html;

PGconn *db;

int main(int argc, char** argv){
    db = PQconnectdb("");
    srand(time(NULL));

    freopen("/srv/eqbeats/eqbeats.log","a",stdout);
    freopen("/srv/eqbeats/eqbeats.log","a",stderr);

    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while(FCGX_Accept_r(&request) == 0){

        resetTimer();
        FCgiIO io(request);
        Cgicc cgi(&io);
        Session::start(cgi);

        // Routing
        int id;
        std::string url = cgi.getEnvironment().getPathInfo();
        if((id = routeUser(url)))
            io << Html::userPage(User(id), cgi("msg"));
        else if((id = routeTrackAction("delete", url)))
            io << Action::deleteTrack(id, cgi);
        else if((id = routeTrackAction("rename", url)))
            io << Action::renameTrack(id, cgi);
        else if((id = routeTrackAction("upload", url)))
            io << Action::uploadTrack(id, cgi);
        else if((id = routeTrackAction("flac", url)))
            io << Html::downloadFlac(id);
        else if((id = routeTrackAction("mp3", url)))
            io << Html::downloadMp3(id);
        else if((id = routeTrackAction("visibility", url)))
            io << Action::trackVisibility(id, cgi);
        else if((id = routeTrack(url)))
            io << Html::trackPage(Track(id), cgi("error"));
        else if(url == "/track/new")
            io << Action::newTrack(cgi);
        else if(url == "/users")
            io << Html::usersPage();
        else if(url == "/artists")
            io << Html::artistsPage();
        else if(url == "/register")
            io << Action::registration(cgi);
        else if(url == "/account")
            io << Action::account(cgi);
        else if(url == "/login")
            io << Action::login(cgi);
        else if(url == "/logout")
            io << Action::logout();
        else if(url == "/")
            io << Html::home();
        else
            io << Html::notFound();

        Session::destroy();
        FCGX_Finish_r(&request);
    }

    PQfinish(db);
    return 0;
}
