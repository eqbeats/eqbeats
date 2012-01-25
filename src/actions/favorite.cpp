#include "actions.h"
#include "../session.h"
#include "../track.h"
#include "../render/html.h"
#include "../render/http.h"
#include "../utils.h"

bool fromEqBeats(cgicc::Cgicc &cgi){
    std::string ref = cgi.getEnvironment().getReferrer();
    if(ref.length() < eqbeatsUrl().length()) return false;
    return ref.substr(0, eqbeatsUrl().length()) == eqbeatsUrl();
}

std::string Action::follow(int uid, bool add, cgicc::Cgicc &cgi){
    if(!fromEqBeats(cgi)) return Http::redirect(User::url(uid));
    User u(uid);
    if(!u) return Html::notFound("User");
    if(!Session::user())
        return Http::redirect("/login?redirect=" + u.url() + "/" + (add?"":"un") + "follow");
    if(Session::user() == u)
        return Http::redirect(u.url());
    add ? Session::user().follow(uid)
        : Session::user().unfollow(uid);
    return Http::redirect(u.url());
}

std::string Action::favorite(int tid, bool add, cgicc::Cgicc &cgi){
    if(!fromEqBeats(cgi)) return Http::redirect(Track::url(tid));
    Track t(tid);
    if(!t) return Html::notFound("Track");
    if(!Session::user())
        return Http::redirect("/login?redirect=" + t.url() + "/" + (add?"":"un") + "favorite");
    add ? Session::user().addToFavorites(tid)
        : Session::user().removeFromFavorites(tid);
    return Http::redirect(t.url());
}
