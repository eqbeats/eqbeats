#include "db.h"
#include "session.h"
#include "utils.h"
#include <cgicc/CgiEnvironment.h>
#include <cgicc/HTTPCookie.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned long int ulong;

using namespace cgicc;

std::string randomSid(){
    ulong n = ((ulong) rand() << 32) | (ulong) rand();
    char str[16];
    sprintf(str, "%016lx", n);
    return std::string(str);
}

User u;
std::string sid;

void Session::start(Cgicc &cgi){
    CgiEnvironment env = cgi.getEnvironment();

    for(std::vector<HTTPCookie>::const_iterator i=env.getCookieList().begin(); i!= env.getCookieList().end(); i++){
        if(i->getName() == "sid")
            sid = i->getValue();
    }

    const char* val[2] = { sid.c_str(), env.getRemoteAddr().c_str() };
    PGresult* result = PQexecParams(db,
        "SELECT user_id FROM sessions WHERE sid = $1 AND host = $2",
        2, 0, val, 0, 0, 0);
    if(PQntuples(result)!=0)
        u = User(atoi(PQgetvalue(result, 0, 0)));
    PQclear(result);
}

void Session::destroy(){
    u = User();
    sid = std::string();
}

User Session::user(){
    return u;
}

std::string Session::login(const std::string &email, const std::string &pw, const std::string &host){
    const char* val[] = { email.c_str(), pw.c_str() };
    PGresult* result = PQexecParams(db,
        "SELECT id FROM users WHERE email = lower($1) AND password = crypt($2, password)"
        , 2, 0, val, 0, 0, 0);

    if(PQntuples(result) == 0) {
        PQclear(result);
        return std::string();
    }
    int id = atoi(PQgetvalue(result, 0, 0));
    PQclear(result);

    return login(id, host);
}

std::string Session::login(int id, const std::string &host){
    sid = randomSid();
    const char* val[3] = { number(id).c_str(), sid.c_str(), host.c_str() };
    PQclear(PQexecParams(db,
        "INSERT INTO sessions (user_id, sid, host, date) "
        "VALUES ($1, $2, $3, 'now')",
        3, 0, val, 0, 0, 0));
    PQclear(PQexecParams(db,
        "UPDATE users SET last_login = 'now' WHERE id = $1",
        1, 0, val, 0, 0, 0));
    return sid;
}

void Session::logout(){
    if(sid.empty()) return;
    const char* val[1] = { sid.c_str() };
    PQclear(PQexecParams(db,
        "DELETE FROM sessions WHERE sid = $1",
        1, 0, val, 0, 0, 0));
    Session::destroy();
}
