#include "pages.h"
#include "../playlist.h"
#include <account/session.h>
#include <core/cgi.h>
#include <log/log.h>
#include <text/text.h>
#include <user_feature/feature.h>

void Pages::playlistActions(Document *doc){

    bool post = cgi.getEnvironment().getRequestMethod() == "POST";
    bool nonce = Session::nonce() == cgi("nonce");

    if(path == "/playlist/new"){
        if(!Session::user())
            return doc->redirect("/");
        std::string name = cgi("name");
        if(name.empty() || !post || !nonce)
            return doc->redirect(Session::user().url());
        Session::newNonce();
        DB::Result r = DB::query(
            "INSERT INTO playlists (user_id, name, description, track_ids) VALUES "
            "("+number(Session::user().id)+", $1, '', ARRAY[]::int[]) RETURNING id", name);
        if(r.empty())
            return doc->redirect(Session::user().url());
        log("New playlist: " + name + " (" + r[0][0] + ")");
        return doc->redirect("/playlist/" + r[0][0] + "?firstrun=1");
    }

    std::string sub;

    int tid = route("track", path, sub);
    if(tid && sub == "playlist"){
        Playlist p(number(cgi("playlist")));
        if(!p) return;
        if(post && p.author().self()){
            Track t(tid);
            if(t && t.visible && nonce){
                Session::newNonce();
                p.add(tid);
            }
        }
        return doc->redirect(p.url());
    }

    int id = route("playlist", path, sub);
    Playlist p(id);
    if(!p) return;

    if(sub == "delete"){
        if(!p.author().self())
            return doc->redirect(p.url());
        if(!post || cgi("confirm") != "Delete" || !nonce){
            doc->setHtml("html/delete.tpl", "Playlist deletion");
            doc->dict()->SetValue("WHAT", p.name());
            doc->dict()->SetValue("CANCEL_URL", p.url());
        }
        else{
            Session::newNonce();
            log("Deleting playlist: " + p.name() + " (" + number(p.id()) + ")");
            DB::query("DELETE FROM playlists WHERE id = " + number(p.id()));
            doc->redirect(Session::user().url());
        }
    }

    else if(sub == "edit"){
        if(!p) return;
        if(post && nonce && (p.name() != cgi("name") || p.description() != cgi("desc"))
           && p.author().self() && !cgi("name").empty()){
            Session::newNonce();
            DB::query("UPDATE playlists SET name = $1, description = $2 WHERE id = " + number(id), cgi("name"), cgi("desc"));
        }
        doc->redirect(p.url());
    }

    else if(sub == "move"){
        if(!p) return;
        std::string dir = cgi("dir");
        if(!post || !p.author().self() || (dir != "up" && dir != "down") || !nonce)
            return doc->redirect(p.url());
        Session::newNonce();
        int i = number(cgi("item"));
        p.swap(i, i + (dir == "up" ? -1 : 1));
        doc->redirect(p.url() + "#tracks");
    }

    else if(sub == "remove"){
        if(post && p.author().self() && nonce){
            Session::newNonce();
            p.remove(number(cgi("item")));
        }
        doc->redirect(p.url() + "#tracks");
    }

    else if(sub == "feature"){
        User u = p.author();
        if(post && u.self() && nonce){
            Session::newNonce();
            if(Feature(u.id))
                DB::query("UPDATE user_features SET ref = "+number(p.id())+", type = 'playlist' WHERE user_id = " + number(u.id));
            else
                DB::query("INSERT INTO user_features (ref, type, user_id) VALUES ("+number(p.id())+", 'playlist', " + number(u.id)+")");
            doc->redirect(u.url());
            return;
        }
        doc->redirect(p.url());
    }

}
