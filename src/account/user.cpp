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

bool User::self() const{
    return Session::user() == *this;
}

std::string User::url() const{
    std::string tmp;
    if(urlize(name, tmp)) {
        return "/user/" + number(id) + "-" + tmp;
    } else {
        return "/user/" + number(id);
    }
}

void User::fill(Dict *d) const{
    if(!id) return;
    d->SetIntValue("UID", id);
    d->SetValue("USERNAME", name);
    d->ShowSection(self() ? "IS_SELF" : "NOT_SELF");

    std::string tmp;
    if(urlize(name, tmp)) {
        d->SetValue("ULINK", number(id) + "-" + tmp);
    } else {
        d->SetValue("ULINK", number(id));
    }

}
