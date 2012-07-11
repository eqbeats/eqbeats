#include "pages.h"
#include "../audio.h"
#include <account/session.h>
#include <core/cgi.h>
#include <core/db.h>
#include <log/log.h>
#include <text/text.h>

void Pages::trackActions(Document *doc){
    std::string sub;
    int tid = route("track", path, sub);
    if(!tid) return;
    bool post = cgi.getEnvironment().getRequestMethod() == "POST";

    if(sub == "rename"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self()){
            std::string title = cgi("title");
            if(!title.empty() && title != t.title){
                DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(t.id), title);
                t.title = title;
                Audio(&t).updateTags();
            }
        }
        doc->redirect(t.url());
    }

    else if(sub == "tags"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self())
            DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(t.id), cgi("tags"));
        doc->redirect(t.url());
    }

    else if(sub == "notes"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self())
            DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(t.id), cgi("notes"));
        doc->redirect(t.url());
    }

    else if(sub == "flags"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self())
            DB::query("UPDATE tracks SET airable = $1 WHERE id = " + number(t.id), cgi.queryCheckbox("airable") ? "t" : "f");
        doc->redirect(t.url());
    }

    else if(sub == "report"){
        Track t(tid);
        if(!t) return;
        if(post){
            std::string path = eqbeatsDir() + "/reports";
            std::ofstream f(path.c_str(), std::ios_base::app);
            f << t.artist.id << " " << t.artist.name << " - " << t.id << " " << t.title << std::endl;
            f.close();
        }
        doc->redirect(t.url());
    }

}
