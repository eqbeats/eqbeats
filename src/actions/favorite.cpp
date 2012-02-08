#include "actions.h"
#include "../session.h"
#include "../track.h"
#include "../render/html.h"
#include "../render/http.h"
#include "../utils.h"
#include "../contest.h"

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

std::string Action::vote(int cid, cgicc::Cgicc &cgi){
    if(!fromEqBeats(cgi)) return Http::redirect(Contest::url(cid));
    Contest c(cid);
    if(!c) return Html::notFound("Contest");
    std::string param = cgi("tid");
    std::string host = cgi.getEnvironment().getRemoteAddr();
    if(!param.empty()){
        int tid = number(param.substr(1));
        if(!tid);
        else if(param[0] == '-')
            c.unvote(tid, host);
        else c.vote(tid, host);
    }
    return Http::redirect(c.url());
}
