#include <account/user.h>
#include <core/cgi.h>
#include <core/db.h>
#include <render/http.h>
#include <render/render.h>
#include <session/session.h>

#include <ctemplate/template.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH(p) if(path == "/" p)
#define SUB(s) if(sub == s)

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

    ctemplate::TemplateCache cache;
    cache.SetTemplateRootDirectory(eqbeatsDir() + "/templates");

    std::string path, sub;
    int id;

    while(FCGX_Accept_r(&request) == 0){
        //resetTimer();
        o.attach(&request);
        cgi = Cgicc(&o);
        path = getPath();

        ctemplate::TemplateDictionary dict("eqbeats");
        std::string tpl, title;
        std::string mime="text/html";

        // Nope
        if (cgi.getElementByValue("PHPE9568F34-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end() ||
            cgi.getElementByValue("PHPE9568F35-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end() ||
            cgi.getElementByValue("PHPE9568F36-D428-11d2-A769-00AA001ACF42") != cgi.getElements().end()){
            //Http::redirect("http://youtu.be/gvdf5n-zI14");
        }

        Session::start();

        // oEmbed
        PATH("oembed") //oEmbed(cgi("url"), cgi("format")=="xml", number(cgi("maxwidth")));

        // User
        if((id = route("user", path, sub))){
            SUB("");
            SUB("comment");
            SUB("follow");
            SUB("unfollow");
            SUB("favorites");
            // Static
            SUB("atom");
            SUB("json");
            SUB("favorites/json");
        }

        // Track
        if((id = route("track", path, sub))){
            SUB("");
            SUB("embed");
            SUB("delete");
            SUB("rename");
            SUB("notes");
            SUB("upload");
            SUB("art/upload");
            SUB("publish");
            SUB("comment");
            SUB("favorite");
            SUB("unfavorite");
            SUB("report");
            SUB("flags");
            SUB("tags");
            SUB("license");
            SUB("youtube_upload");
            SUB("playlist");
            // Static
            SUB("original");
            SUB("vorbis");
            SUB("mp3");
            SUB("art");
            SUB("art/medium");
            SUB("art/thumb");
            SUB("json");
        }

        // Tracks
        PATH("track/new");
        PATH("tracks"); //Http::redirect("/");
        PATH("tracks/search");
        PATH("tracks/latest");
        PATH("tracks/random");
        PATH("tracks/featured");
        PATH("tracks/search/json");
        PATH("tracks/search/exact/json");
        PATH("tracks/latest/json");
        PATH("tracks/random/json");
        PATH("tracks/featured/json");
        PATH("tracks/latest/atom");
        PATH("tracks/featured/atom");
        if(path.substr(0,12) == "/tracks/tag/"){
            //std::string tag = path.substr(12);
            //Html::tracksPage(tag, Track::byTag(tag));
        }

        // News
        if((id = route("news", path, sub))){
            SUB("");
            SUB("comment");
        }
        PATH("news");

        // Contests
        if((id = route("contest", path, sub))){
            SUB("");
            SUB("submit");
            SUB("vote");
        }

        // Playlists
        else if((id = route("playlist", path, sub))){
            SUB("");
            SUB("remove");
            SUB("move");
            SUB("delete");
            SUB("edit");
            SUB("json");
        }
        PATH("playlist/new");

        // Users
        PATH("users/search");
        PATH("users/search/json");
        PATH("artists/json");
        PATH("artists");

        // Account
        PATH("register");
        PATH("account");
        PATH("account/reset");
        PATH("account/license");
        PATH("login");
        PATH("logout");
        PATH("oauth/yt/unlink");
        PATH("oauth/yt");

        // Static
        PATH("quickstart") title = "Thanks", tpl = "quickstart.tpl";
        PATH("faq") title = "FAQ", tpl = "faq.tpl";
        PATH("credits") title = "Credits", tpl = "credits.tpl";
        PATH("api") title = "API", tpl = "api.tpl";
        PATH("") tpl = "home.tpl";

        // Render
        if(!tpl.empty() && mime=="text/html"){
            ctemplate::TemplateDictionary *body = dict.AddIncludeDictionary("BODY");
            body->SetFilename(tpl);
            dict.SetValueAndShowSection("TITLE", title, "HAS_TITLE");
            if(Session::user()){
                ctemplate::TemplateDictionary *u = dict.AddSectionDictionary("LOGGED_USER");
                u->SetValue("URL", Session::user().url());
                u->SetValue("NAME", Session::user().name());
            }
            else
                dict.ShowSection("LOGGED_OUT");
            std::string out;
            cache.ExpandWithData("page.tpl", ctemplate::STRIP_BLANK_LINES, &dict, NULL, &out);
            o << Http::header(mime, 200) << out;
        }

        Session::destroy();
        FCGX_Finish_r(&request);
        while(waitpid(-1, NULL, WNOHANG) > 0); // wait for zombies
    }

    DB::close();
    return 0;
}
