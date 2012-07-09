#include <account/account.h>
#include <account/list.h>
#include <core/cgi.h>
#include <core/db.h>
#include <core/template.h>
#include <media/art.h>
#include <misc/timer.h>
#include <render/fcgiio.h>
#include <render/http.h>
#include <render/static.h>
#include <text/modifiers.h>
#include <text/text.h>
#include <track/audio.h>
#include <track/extended.h>
#include <track/list.h>
#include <session/session.h>

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace cgicc;
Cgicc cgi;
std::string path;

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
    ctemplate::AddModifier("x-email", new EmailEscape);
    ctemplate::AddXssSafeModifier("x-format", new Formatter);
    cache.SetTemplateRootDirectory(eqbeatsDir() + "/templates");

    while(FCGX_Accept_r(&request) == 0){
        resetTimer();
        o.attach(&request);
        cgi = Cgicc(&o);
        path = stripSlash(cgi.getEnvironment().getPathInfo());
        Document doc;

        // Nope
        if (cgi.getElementByValue("PHPE9568F34-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end() ||
            cgi.getElementByValue("PHPE9568F35-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end() ||
            cgi.getElementByValue("PHPE9568F36-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end()){
            doc.redirect("http://youtu.be/gvdf5n-zI14");
        }

        Session::start();

        staticPages(&doc);
        o << doc.generate();

        Session::destroy();
        FCGX_Finish_r(&request);
        while(waitpid(-1, NULL, WNOHANG) > 0); // wait for zombies
    }

    DB::close();
    return 0;
}
