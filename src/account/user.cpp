#include "user.h"
#include "session.h"
#include <core/db.h>
#include <text/text.h>

User::User(int uid){
    id = 0;
    if(uid<=0) return;
    DB::Result r = DB::query("SELECT name FROM users WHERE id = " + number(uid));
    if(!r.empty()){
        id = uid;
        name = r[0][0];
    }
}

User::User(const std::string &email){
    id = 0;
    DB::Result r = DB::query("SELECT id, name FROM users WHERE email = $1", email);
    if(!r.empty()){
        id = number(r[0][0]);
        name = r[0][1];
    }
}

std::string User::url() const{
    return "/user/" + number(id);
}

void User::fill(Dict *d) const{
    d->SetIntValue("UID", id);
    d->SetValue("USERNAME", name);
    if(Session::user() == *this)
        d->ShowSection("IS_SELF");
}
