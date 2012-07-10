#include "pages.h"
#include "../account.h"
#include <core/cgi.h>

void Pages::user(Document *doc){

    std::string sub;
    int uid = route("user", path, sub);

    Account u(uid);
    if(!u) return;

    if(sub == ""){
        doc->setHtml("html/user.tpl", u.name);
        u.fill(doc->dict());
    }

}
