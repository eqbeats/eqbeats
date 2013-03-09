#include "pages.h"
#include <account/user.h>
#include <core/cgi.h>
#include <core/db.h>
#include <text/text.h>
#include <account/session.h>

#include <log/log.h>

void Pages::userActions(Document* doc){
    std::string sub;
    int uid = route("user", path, sub);
    User u(uid);
    if(!u) return;
    bool post = cgi.getEnvironment().getRequestMethod() == "POST";
    bool nonce = Session::nonce() == cgi("nonce");
    if(sub == "defeature" && u.self() && post && nonce)
        DB::query("DELETE FROM user_features WHERE user_id = " + number(u.id));
    doc->redirect(u.url());
}
