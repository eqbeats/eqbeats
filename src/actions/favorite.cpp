#include "actions.h"
#include "../session.h"
#include "../track.h"
#include "../render/http.h"
#include "../render/html/page.h"
#include "../number.h"
#include "../path.h"
#include "../follower.h"
#include "../contest.h"
#include "../cgi.h"

using namespace Render;

bool fromEqBeats(){
    std::string ref = cgi.getEnvironment().getReferrer();
    if(ref.length() < eqbeatsUrl().length()) return false;
    return ref.substr(0, eqbeatsUrl().length()) == eqbeatsUrl();
}

void Action::follow(int uid, bool add){
    if(!fromEqBeats()) return Http::redirect(User::url(uid));
    User u(uid);
    if(!u)
        Html::notFound("User");
    else if(!Session::user())
        Http::redirect("/login?redirect=" + u.url() + "/" + (add?"":"un") + "follow");
    else if(Session::user() == u)
        Http::redirect(u.url());
    else{
        add ? Follower(Session::user()).follow(uid)
            : Follower(Session::user()).unfollow(uid);
        Http::redirect(u.url());
    }
}

void Action::favorite(int tid, bool add){
    if(!fromEqBeats()) return Http::redirect(Track::url(tid));
    Track t(tid);
    if(!t)
        Html::notFound("Track");
    else if(!Session::user())
        Http::redirect("/login?redirect=" + t.url() + "/" + (add?"":"un") + "favorite");
    else{
        add ? Follower(Session::user()).addToFavorites(tid)
            : Follower(Session::user()).removeFromFavorites(tid);
        Http::redirect(t.url());
    }
}

void Action::vote(int cid){
    if(!fromEqBeats()) return Http::redirect(Contest::url(cid));
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
    Http::redirect(c.url()+"#submissions");
}
