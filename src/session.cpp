#include "session.h"
#include "number.h"
#include "db.h"
#include "user.h"
#include "cgi.h"
#include <cgicc/CgiEnvironment.h>
#include <cgicc/HTTPCookie.h>
#include <stdio.h>
#include <string.h>

typedef unsigned long int ulong;

using namespace cgicc;

std::string Session::randomString(){
    ulong n = ((ulong) rand() << 32) | (ulong) rand();
    char str[16];
    sprintf(str, "%016lx", n);
    return std::string(str);
}

User u;
std::string sid;

void Session::start(){
    CgiEnvironment env = cgi.getEnvironment();
    for(std::vector<HTTPCookie>::const_iterator i=env.getCookieList().begin(); i!= env.getCookieList().end(); i++){
        if(i->getName() == "sid")
            sid = i->getValue();
    }
    if(sid.empty()) return;
    DB::Result r = DB::query("SELECT user_id FROM sessions WHERE sid = $1 AND host = $2", sid, env.getRemoteAddr());
    if(!r.empty()) u = User(number(r[0][0]));
}

void Session::destroy(){
    u = User();
    sid = std::string();
}

User Session::user(){
    return u;
}

std::string Session::login(const std::string &email, const std::string &pw, const std::string &host){
    DB::Result r = DB::query("SELECT id FROM users WHERE lower(email) = lower($1) AND password = crypt($2, password)", email, pw);
    return r.empty() ? std::string() : login(number(r[0][0]), host);
}

std::string Session::login(int id, const std::string &host){
    sid = randomString();
    DB::query("INSERT INTO sessions (user_id, sid, host, date) VALUES ($1, $2, $3, 'now')", number(id), sid, host);
    DB::query("UPDATE users SET last_login = 'now' WHERE id = " + number(id));
    u = User(id);
    return sid;
}

void Session::logout(){
    if(sid.empty()) return;
    DB::query("DELETE FROM sessions WHERE sid = $1", sid);
    Session::destroy();
}
