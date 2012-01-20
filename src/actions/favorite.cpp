#include "actions.h"
#include "../session.h"
#include "../track.h"
#include "../html/html.h"
#include "../utils.h"

using namespace Html;

bool fromEqBeats(cgicc::Cgicc &cgi){
    std::string ref = cgi.getEnvironment().getReferrer();
    if(ref.length() < eqbeatsUrl().length()) return false;
    return ref.substr(0, eqbeatsUrl().length()) == eqbeatsUrl();
}

std::string Action::follow(int uid, bool add, cgicc::Cgicc &cgi){
    if(!fromEqBeats(cgi)) return redirect(User::url(uid));
    User u(uid);
    if(!u) return notFound("User");
    if(!Session::user())
        return redirect("/login?redirect=" + u.url() + "/" + (add?"":"un") + "follow");
    if(Session::user() == u)
        return redirect(u.url());
    add ? Session::user().follow(uid)
        : Session::user().unfollow(uid);
    return redirect(u.url());
}

std::string Action::favorite(int tid, bool add, cgicc::Cgicc &cgi){
    if(!fromEqBeats(cgi)) return redirect(Track::url(tid));
    Track t(tid);
    if(!t) return notFound("Track");
    if(!Session::user())
        return redirect("/login?redirect=" + t.url() + "/" + (add?"":"un") + "favorite");
    add ? Session::user().addToFavorites(tid)
        : Session::user().removeFromFavorites(tid);
    return redirect(t.url());
}
