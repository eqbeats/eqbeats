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

    std::string path;
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
        // Static
        else if((id = route("track", "original", path)));
        else if((id = route("track", "vorbis", path)));
        else if((id = route("track", "mp3", path)));
        else if((id = route("track", "art", path)));
        else if((id = route("track", "art/medium", path)));
        else if((id = route("track", "art/thumb", path)));
        // Json
        else if((id = route("track", "json", path)));
        else if((id = route("user", "json", path)));
        else if((id = route("user", "favorites/json", path)));
        else if((id = route("playlist", "json", path)));
        else if(path == "/tracks/search/json");
        else if(path == "/tracks/search/exact/json");
        else if(path == "/tracks/latest/json");
        else if(path == "/tracks/random/json");
        else if(path == "/tracks/featured/json");
        else if(path == "/artists/json");
        else if(path == "/users/search/json");
        // Feeds
        else if(path == "/tracks/latest/atom");
        else if(path == "/tracks/featured/atom");
        else if((id = route("user", "atom", path)));
        // oEmbed
        else if(path == "/oembed");
            //oEmbed(cgi("url"), cgi("format")=="xml", number(cgi("maxwidth")));

        else{
        Session::start();
        // User
        if((id = route("user", path)));
        else if((id = route("user", "comment", path)));
        else if((id = route("user", "follow", path)));
        else if((id = route("user", "unfollow", path)));
        else if((id = route("user", "favorites", path)));
        // Track
        else if((id = route("track", "embed", path)));
        else if((id = route("track", "delete", path)));
        else if((id = route("track", "rename", path)));
        else if((id = route("track", "notes", path)));
        else if((id = route("track", "upload", path)));
        else if((id = route("track", "art/upload", path)));
        else if((id = route("track", "publish", path)));
        else if((id = route("track", "comment", path)));
        else if((id = route("track", "favorite", path)));
        else if((id = route("track", "unfavorite", path)));
        else if((id = route("track", "report", path)));
        else if((id = route("track", "flags", path)));
        else if((id = route("track", "tags", path)));
        else if((id = route("track", "license", path)));
        else if((id = route("track", "youtube_upload", path)));
        else if((id = route("track",path)));
        else if(path == "/track/new");
        // Tracks
        else if(path == "/tracks");
            //Http::redirect("/");
        else if(path == "/tracks/search");
        else if(path == "/tracks/latest");
        else if(path == "/tracks/random");
        else if(path == "/tracks/featured");
        else if(path.substr(0,12) == "/tracks/tag/"){
            //std::string tag = path.substr(12);
            //Html::tracksPage(tag, Track::byTag(tag));
        }
        // News
        else if((id = route("news", path)));
        else if((id = route("news", "comment", path)));
        else if(path == "/news");
        // Contests
        else if((id = route("contest", path)));
        else if((id = route("contest", "submit", path)));
        else if((id = route("contest", "vote", path)));
        // Playlists
        else if((id = route("playlist", path)));
        else if((id = route("track", "playlist", path)));
        else if((id = route("playlist", "remove", path)));
        else if((id = route("playlist", "move", path)));
        else if((id = route("playlist", "delete", path)));
        else if((id = route("playlist", "edit", path)));
        else if(path == "/playlist/new");
        // Users
        else if(path == "/users/search");
        else if(path == "/artists");
        // Actions
        else if(path == "/register");
        else if(path == "/account");
        else if(path == "/account/reset");
        else if(path == "/account/license");
        else if(path == "/login");
        else if(path == "/logout");
        else if(path == "/oauth/yt/unlink");
        else if(path == "/oauth/yt");
        // Static
        else if(path == "/quickstart")
            title = "Thanks", tpl = "quickstart.tpl";
        else if(path == "/faq")
            title = "FAQ", tpl = "faq.tpl";
        else if(path == "/credits")
            title = "Credits", tpl = "credits.tpl";
        else if(path == "/api")
            title = "API", tpl = "api.tpl";
        else if(path == "");
        else
            ;//Html::notFound();

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
        }

        FCGX_Finish_r(&request);
        while(waitpid(-1, NULL, WNOHANG) > 0); // wait for zombies
    }

    DB::close();
    return 0;
}
