#include "session.h"
#include <core/cgi.h>
#include <core/db.h>
#include <misc/hash.h>
#include <text/text.h>

#include <cgicc/CgiEnvironment.h>
#include <cgicc/HTTPCookie.h>

// Internal variables
User u;
std::string sid;
std::string nonce_;

User Session::user(){ return u; }

// Session initialization

void Session::start(){
    // Read the sid cookie
    cgicc::CgiEnvironment env = cgi.getEnvironment();
    for(std::vector<cgicc::HTTPCookie>::const_iterator i=env.getCookieList().begin(); i!= env.getCookieList().end(); i++){
        if(i->getName() == "sid")
            sid = i->getValue();
    }
    if(sid.empty()) return; // No cookie
    DB::Result r = DB::query("SELECT users.id, users.name, sessions.nonce FROM sessions, users WHERE sid = $1 AND host = $2 AND users.id = sessions.user_id", sid, env.getRemoteAddr());
    if(!r.empty()){
        u = User(number(r[0][0]), r[0][1]);
        nonce_ = r[0][2];
    }
}

void Session::destroy(){
    u = User();
    sid = std::string();
    nonce_ = std::string();
}

std::string Session::nonce(){
    if(nonce_.empty()){
        newNonce();
    }
    return nonce_;
}

void Session::newNonce(){
    if(u)
        nonce_ = randomString();
        DB::query("UPDATE sessions SET nonce = $1 WHERE sid = $2 AND host = $3", nonce_, sid, cgi.getEnvironment().getRemoteAddr());
}

// Login / Logout

std::string Session::login(const User &_user){
    u = _user;
    sid = randomString(64);
    DB::query("INSERT INTO sessions (user_id, sid, host, date) VALUES ($1, $2, $3, 'now')", number(u.id), sid, cgi.getEnvironment().getRemoteAddr());
    DB::query("UPDATE users SET last_login = 'now' WHERE id = " + number(u.id));
    return sid;
}

std::string Session::login(const std::string &email, const std::string &pw){
    DB::Result r = DB::query("SELECT id, name FROM users WHERE lower(email) = lower($1) AND password = crypt($2, password)", email, pw);
    return r.empty() ? std::string() : login(User(number(r[0][0]), r[0][1]));
}

void Session::logout(){
    if(sid.empty()) return;
    DB::query("DELETE FROM sessions WHERE sid = $1 AND host = $2", sid, cgi.getEnvironment().getRemoteAddr());
    Session::destroy();
}

// Dict

Dict* Session::fill(Dict *d){
    if(u){
        Dict *s = d->AddSectionDictionary("LOGGED_USER");
        u.fill(s);
        d->SetTemplateGlobalValue("NONCE", nonce());
        return s;
    }
    else
        d->ShowSection("LOGGED_OUT");
    return 0;
}
