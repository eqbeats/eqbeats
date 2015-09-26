#include "pages.h"
#include "../audio.h"
#include <account/session.h>
#include <core/cgi.h>
#include <core/db.h>
#include <log/log.h>
#include <text/text.h>
#include <syslog.h>

void Pages::trackActions(Document *doc){
    std::string sub;
    int tid = route("track", path, sub);
    if(!tid) return;
    bool post = cgi.getEnvironment().getRequestMethod() == "POST";
    bool nonce = Session::nonce() == cgi("nonce");


    if(sub == "rename"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            std::string title = cgi("title");
            if(!validString(title))
                return doc->badRequest();
            if(!title.empty() && title != t.title){
                DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(t.id), title);
                syslog(LOG_NOTICE, "Renaming track %d from \"%s\" to \"%s\".", t.id, t.title.c_str(), title.c_str());
                t.title = title;
                Audio(&t).updateTags();
            }
        }
        doc->redirect(t.url());
    }

    else if(sub == "tags"){
        std::string tags = cgi("tags");
        if(!validString(tags))
            return doc->badRequest();
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(t.id), tags);
        }
        doc->redirect(t.url());
    }

    else if(sub == "notes"){
        std::string notes = cgi("notes");
        if(!validString(notes))
            return doc->badRequest();
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(t.id), notes);
        }
        doc->redirect(t.url());
    }

    else if(sub == "flags"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            DB::query("UPDATE tracks SET airable = $1 WHERE id = " + number(t.id), cgi.queryCheckbox("airable") ? "t" : "f");
        }
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
