#include "pages.h"
#include "../playlist.h"
#include <account/session.h>
#include <core/cgi.h>
#include <log/log.h>
#include <text/text.h>

void Pages::playlistActions(Document *doc){

    bool post = cgi.getEnvironment().getRequestMethod() == "POST";

    if(path == "/playlist/new"){
        if(!Session::user())
            return doc->redirect("/");
        std::string name = cgi("name");
        if(name.empty() || !post)
            return doc->redirect(Session::user().url());
        DB::Result r = DB::query(
            "INSERT INTO playlists (user_id, name, description, track_ids) VALUES "
            "("+number(Session::user().id)+", $1, '', ARRAY[]::int[]) RETURNING id", name);
        if(r.empty())
            return doc->redirect(Session::user().url());
        log("New playlist: " + name + " (" + r[0][0] + ")");
        return doc->redirect("/playlist/" + r[0][0]);
    }

    std::string sub;

    int tid = route("track", path, sub);
    if(tid && sub == "playlist"){
        Playlist p(number(cgi("playlist")));
        if(!p) return;
        if(post && Session::user() == p.author()){
            Track t(tid);
            if(t && t.visible)
                p.add(tid);
        }
        return doc->redirect(p.url());
    }

    int id = route("playlist", path, sub);
    if(!id) return;

    if(sub == "delete"){
        Playlist p(id);
        if(!p) return;
        if(p.author() != Session::user() || !post || cgi("confirm") != "Delete")
            return doc->redirect(p.url());
        log("Deleting playlist: " + p.name() + " (" + number(p.id()) + ")");
        DB::query("DELETE FROM playlists WHERE id = " + number(p.id()));
        doc->redirect(Session::user().url());
    }

    else if(sub == "edit"){
        Playlist p(id);
        if(!p) return;
        if(post && (p.name() != cgi("name") || p.description() != cgi("desc"))
           && p.author() == Session::user() && !cgi("name").empty())
            DB::query("UPDATE playlists SET name = $1, description = $2 WHERE id = " + number(id), cgi("name"), cgi("desc"));
        doc->redirect(p.url());
    }

    else if(sub == "move"){
        Playlist p(id);
        if(!p) return;
        std::string dir = cgi("dir");
        if(!post || Session::user() != p.author() || (dir != "up" && dir != "down"))
            return doc->redirect(p.url());
        int i = number(cgi("item"));
        p.swap(i, i + (dir == "up" ? -1 : 1));
        doc->redirect(p.url() + "#tracks");
    }

    else if(sub == "remove"){
        Playlist p(id);
        if(!p) return;
        if(post && p.author() == Session::user())
            p.remove(number(cgi("item")));
    }

}
