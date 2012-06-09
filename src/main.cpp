#include <account/user.h>
#include <core/cgi.h>
#include <core/db.h>
#include <core/template.h>
#include <media/art.h>
#include <media/audio.h>
#include <misc/timer.h>
#include <render/fcgiio.h>
#include <render/http.h>
#include <text/modifiers.h>
#include <text/text.h>
#include <track/track.h>
#include <track/list.h>
#include <session/session.h>

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH(p) if(path == (p))
#define ROUTE(t) if((id = route((t), path, sub)))
#define SUB(s) if(sub == (s))

#define HTML(t) \
    { mime = "text/html"; \
      rootDict->SetValueAndShowSection("TITLE", (t), "HAS_TITLE"); \
      dict = rootDict->AddIncludeDictionary("BODY"); }
#define JSON()\
    { mime = "application/json"; \
      dict = rootDict->AddIncludeDictionary("JSON"); }

#define LOGGED if(!Session::user()) redir = "/login?redirect=" + path; else

using namespace cgicc;
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
    FCgiIO o;

    ctemplate::AddModifier("x-irc", new IrcEscape);
    ctemplate::TemplateCache cache;
    cache.SetTemplateRootDirectory(eqbeatsDir() + "/templates");

    std::string path, sub;
    int id;

    while(FCGX_Accept_r(&request) == 0){
        resetTimer();
        o.attach(&request);
        cgi = Cgicc(&o);
        path = getPath();

        Dict *rootDict = new Dict("eqbeats");
        Dict *dict = rootDict;
        dict->SetGlobalValue("EQBEATS_URL", eqbeatsUrl());
        std::string tpl, redir, mime = "text/html";
        int code = 200;

        // Nope
        if (cgi.getElementByValue("PHPE9568F34-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end() ||
            cgi.getElementByValue("PHPE9568F35-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end() ||
            cgi.getElementByValue("PHPE9568F36-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end()){
            redir = "http://youtu.be/gvdf5n-zI14";
        }

        Session::start();

        #include "pages/home.cpp"
        #include "pages/session.cpp"
        #include "pages/static.cpp"
        #include "pages/track.cpp"
        #include "pages/tracks.cpp"

        //#include "pages/oembed.cpp"
        //#include "pages/user.cpp"
        //#include "pages/news.cpp"
        //#include "pages/contest.cpp"
        //#include "pages/playlist.cpp"
        //#include "pages/users.cpp"
        //#include "pages/account.cpp"

        // Render
        if(!redir.empty())
            o << Http::redirect(redir);

        else if(code == 200){
            std::string out;
            if(tpl.empty() && mime == "text/html"){ // Defaults
                HTML("404 Not Found");
                code = 404;
                tpl = "404.tpl";
            }
            if(dict != rootDict && mime == "text/html"){ // HTML macro called
                dict->SetFilename(tpl);
                Session::fill(rootDict);
                rootDict->SetValueAndShowSection("REDIRECT", path, "HAS_REDIRECT");
                rootDict->SetFormattedValue("GENERATION_TIME", "%lu µS", usecs());
                rootDict->SetFormattedValue("PID", "%d", getpid());
                cache.ExpandWithData("page.tpl", ctemplate::STRIP_BLANK_LINES, rootDict, NULL, &out);
            }
            else if(dict != rootDict && mime == "application/json"){ // JSON macro
                if(!cgi("jsonp").empty()){
                    mime = "text/javascript";
                    rootDict->SetValueAndShowSection("FUNCTION", cgi("jsonp"), "JSONP");
                }
                dict->SetFilename(tpl);
                cache.ExpandWithData("jsonp.tpl", ctemplate::STRIP_WHITESPACE, rootDict, NULL, &out);
            }
            else if(!tpl.empty())
                cache.ExpandWithData(tpl, ctemplate::STRIP_BLANK_LINES, rootDict, NULL, &out);
            o << Http::header(mime, code) << out;
        }


        Session::destroy();
        FCGX_Finish_r(&request);
        delete rootDict;
        while(waitpid(-1, NULL, WNOHANG) > 0); // wait for zombies
    }

    DB::close();
    return 0;
}
