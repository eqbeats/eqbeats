#include "pages.h"
#include "../event.h"
#include "../follower.h"
#include <account/session.h>
#include <core/cgi.h>
#include <core/db.h>
#include <text/text.h>

#include <pcrecpp.h>

bool fromEqBeats(){
    return pcrecpp::RE(
        eqbeatsUrl() + "/.*",
        pcrecpp::RE_Options().set_caseless(true)
    ).FullMatch(cgi.getEnvironment().getReferrer());
}

void Pages::socialActions(Document *doc){

    std::string sub, _sub;
    int uid = route("user", path, sub);
    int tid = route("track", path, _sub);

    if((uid && (sub == "follow" || sub == "unfollow")) ||
       (tid && (_sub == "favorite" || _sub == "unfavorite"))){

        User self = Session::user();
        Follower f(self.id);
        User u(uid);
        Track t(tid);
        if(!u && !t) return;

        if(fromEqBeats() && self && Session::nonce() == cgi("nonce")){
            Session::newNonce();

            if(t){
                bool fav = f.favorited(t.id);
                if(_sub == "favorite" && !fav){
                    DB::query(
                        "INSERT INTO favorites (user_id, type, ref) "
                        "VALUES (" + number(self.id) + ", 'track', " + number(t.id) + ")"
                    );
                    Event e;
                    e.type = Event::Favorite;
                    e.source = self;
                    e.target = t.artist;
                    e.track = t;
                    e.push();
                }
                else if(_sub == "unfavorite" && fav){
                    DB::query("DELETE FROM favorites WHERE type = 'track' "
                        "AND ref = " + number(t.id) + " AND user_id = " + number(self.id));
                    DB::query("DELETE FROM events WHERE type = 'favorite' "
                        "AND source_id = " + number(self.id) + " AND track_id = " + number(t.id));
                }
            }

            else if(u && !u.self()){
                bool followed = f.following(u.id);
                if(sub == "follow" && !followed){
                    DB::query("INSERT INTO favorites (user_id, type, ref) "
                        "VALUES (" + number(self.id) + ", 'artist', " + number(u.id) + ")");
                    Event e;
                    e.type = Event::Follow;
                    e.source = self;
                    e.target = u;
                    e.push();
                }
                else if(sub == "unfollow" && followed){
                    DB::query("DELETE FROM favorites WHERE type = 'artist' "
                        "AND ref = " + number(u.id) + " AND user_id = " + number(self.id));
                    DB::query("DELETE FROM events WHERE type = 'follow' "
                        "AND source_id = " + number(self.id) + " AND target_id = " + number(u.id));
                }
            }

        }

        doc->redirect(u ? u.url() : t.url());

    }

}
