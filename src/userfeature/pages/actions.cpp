#include "pages.h"
#include <core/cgi.h>
#include <core/db.h>
#include <text/text.h>
#include <account/user.h>
#include <account/session.h>
#include <track/track.h>
#include <playlist/playlist.h>
#include <userfeature/feature.h>

void Pages::featureActions(Document* doc){
    std::string sub, _sub, __sub;
    int uid = route("user", path, sub);
    int tid = route("track", path, _sub);
    int pid = route("playlist", path, __sub);
    bool post = cgi.getEnvironment().getRequestMethod() == "POST";
    bool nonce = Session::nonce() == cgi("nonce");
    if(uid && sub == "defeature"){
        User u(uid);
        if(!u) return;
        if(sub == "defeature" && u.self() && post && nonce)
            DB::query("DELETE FROM user_features WHERE user_id = " + number(u.id));
        doc->redirect(u.url());
    } else if(tid && _sub == "feature"){
        Track t(tid);
        if(!t) return;
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
    } else if(pid && __sub == "feature"){
        Playlist p(pid);
        if(!p) return;
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
