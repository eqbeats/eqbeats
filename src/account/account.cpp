#include "account.h"
#include "session.h"
#include <core/db.h>
#include <misc/hash.h>
#include <text/text.h>
#include <youtube/youtube.h>

Account::Account(int uid){
    id = 0;
    if(uid<=0) return;
    DB::Result r = DB::query("SELECT name, email, about, notify, license FROM users WHERE id=" + number(uid));
    if(!r.empty()){
        id = uid;
        name = r[0][0];
        email = r[0][1];
        about = r[0][2];
        notify = r[0][3] == "t";
        license = r[0][4];
    }

    // Using LEFT JOIN / GROUP BY above would require grouping by all otherwise
    // selected attributes, we're probably better off this way.
    r = DB::query("SELECT COUNT(*) FROM favorites WHERE type = 'track' AND user_id = " + number(uid));
    num_favs = r.empty() ? 0 : number(r[0][0]);

}

void Account::fill(Dict* d) const{
    User::fill(d);
    d->SetValue("EMAIL", email);
    d->SetValueAndShowSection("ABOUT", about, "HAS_ABOUT");
    d->SetValue("LICENSE", license);
    if(notify) d->ShowSection("NOTIFY");
    // Avatar
    Dict *a = d->AddIncludeDictionary("AVATAR");
    a->SetFilename("html/avatar.tpl");
    a->SetValue("EMAIL_MD5", md5(email));
    if(self()) a->ShowSection("IS_SELF");
    // YouTube
    d->ShowSection(Youtube(id) ? "YOUTUBE" : "NO_YOUTUBE");
    // Number of favorites
    if(num_favs > 0) {
        d->ShowSection("HAS_FAVS");
        d->SetValue("NUM_FAVS", number(num_favs));
    }
}
