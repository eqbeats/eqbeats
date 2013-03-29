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
    bool nonce = Session::nonce() == cgi("nonce");


    if(sub == "rename"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self() && nonce){
            Session::newNonce();
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
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(t.id), cgi("tags"));
        }
        doc->redirect(t.url());
    }

    else if(sub == "notes"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(t.id), cgi("notes"));
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

void Pages::JSONTrackActions(Document *doc){
    std::string sub;
    int tid = route("track", path, sub);
    if(!tid) return;
    bool post = cgi.getEnvironment().getRequestMethod() == "POST";

    if(sub == "rename/json" || sub == "tags/json" || sub == "notes/json" || sub == "flags/json"){
        if(!post){
            doc->setJson("json/error.tpl", 405);
            doc->dict()->SetValue("ERROR", "This resource can only be accessed with POST.");
            return;
        }
        Track t(tid);
        if(!t){
            doc->setJson("json/error.tpl", 404);
            doc->dict()->SetValue("ERROR", "No such track.");
            return;
        }
        if(!t.artist.self()){
            doc->setJson("json/error.tpl", 403);
            doc->dict()->SetValue("ERROR", "You do not own this track.");
            return;
        }
        if(sub == "rename/json"){
            std::string title = cgi("title");
            if(!title.empty() && title != t.title){
                DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(t.id), title);
                t.title = title;
                Audio(&t).updateTags();
            }
        }
        else if(sub == "tags/json")
            DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(tid), cgi("tags"));
        else if(sub == "notes/json")
            DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(tid), cgi("notes"));
        else if(sub == "flags/json")
            DB::query("UPDATE tracks SET airable = $1 WHERE id = " + number(t.id), cgi("airable"));
        doc->setJson("json/ok.tpl", 200);
    }
}
