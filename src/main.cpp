#include <account/pages/pages.h>
#include <account/session.h>
#include <core/cgi.h>
#include <core/db.h>
#include <misc/timer.h>
#include <pages/pages.h>
#include <playlist/pages/pages.h>
#include <render/document.h>
#include <render/fcgiio.h>
#include <social/pages/pages.h>
#include <text/modifiers.h>
#include <track/pages/pages.h>

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

cgicc::Cgicc cgi;
std::string path;
ctemplate::TemplateCache cache;

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
    FCgiIO o;

    ctemplate::AddModifier("x-irc", new IrcEscape);
    ctemplate::AddXssSafeModifier("x-email", new EmailEscape);
    ctemplate::AddXssSafeModifier("x-format", new Formatter);
    cache.SetTemplateRootDirectory(eqbeatsDir() + "/templates");

    void (*callbacks[])(Document*) = {
        Pages::statics, Pages::home,
        Pages::track, Pages::tracks, Pages::oembed, Pages::trackActions, Pages::trackUpload, Pages::art,
        Pages::account, Pages::login, Pages::user, Pages::users, Pages::registration, Pages::passwordReset,
        Pages::playlist, Pages::playlistActions,
        Pages::comment, Pages::socialActions,
        0
    };

    while(FCGX_Accept_r(&request) == 0){
        resetTimer();
        o.attach(&request);
        cgi = cgicc::Cgicc(&o);
        path = stripSlash(cgi.getEnvironment().getPathInfo());
        Document doc;

        // Nope
        if (cgi.getElementByValue("PHPE9568F34-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end() ||
            cgi.getElementByValue("PHPE9568F35-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end() ||
            cgi.getElementByValue("PHPE9568F36-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end()){
            doc.redirect("http://youtu.be/gvdf5n-zI14");
        }

        Session::start();

        for(int i=0; !doc && callbacks[i]; i++)
            callbacks[i](&doc);

        Session::fill(doc.rootDict());
        o << doc.generate();

        Session::destroy();
        FCGX_Finish_r(&request);
        while(waitpid(-1, NULL, WNOHANG) > 0); // wait for zombies
    }

    DB::close();
    return 0;
}
