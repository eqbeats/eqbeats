#include "pages.h"
#include "../audio.h"
#include <account/session.h>
#include <core/cgi.h>
#include <core/db.h>
#include <log/log.h>
#include <text/text.h>
#include <user_feature/feature.h>

void Pages::trackActions(Document *doc){
    std::string sub;
    int tid = route("track", path, sub);
    Track t(tid);
    if(!t) return;
    bool post = cgi.getEnvironment().getRequestMethod() == "POST";
    bool nonce = Session::nonce() == cgi("nonce");


    if(sub == "rename"){
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
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(t.id), cgi("tags"));
        }
        doc->redirect(t.url());
    }

    else if(sub == "notes"){
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(t.id), cgi("notes"));
        }
        doc->redirect(t.url());
    }

    else if(sub == "flags"){
        if(post && t.artist.self() && nonce){
            Session::newNonce();
            DB::query("UPDATE tracks SET airable = $1 WHERE id = " + number(t.id), cgi.queryCheckbox("airable") ? "t" : "f");
        }
        doc->redirect(t.url());
    }

    else if(sub == "report"){
        if(post){
            std::string path = eqbeatsDir() + "/reports";
            std::ofstream f(path.c_str(), std::ios_base::app);
            f << t.artist.id << " " << t.artist.name << " - " << t.id << " " << t.title << std::endl;
            f.close();
        }
        doc->redirect(t.url());
    }

    else if(sub == "feature"){
        User u = t.artist;
        if(post && u.self() && nonce){
            Session::newNonce();
            if(Feature(u.id))
                DB::query("UPDATE user_features SET ref = "+number(t.id)+", type = 'track' WHERE user_id = " + number(u.id));
            else
                DB::query("INSERT INTO user_features (ref, type, user_id) VALUES ("+number(t.id)+", 'track', " + number(u.id)+")");
            doc->redirect(u.url());
            return;
        }
        doc->redirect(t.url());
    }
}
