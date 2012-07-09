#include "pages.h"
#include "../account.h"
#include <core/cgi.h>
#include <render/document.h>

namespace Pages {

void account(Document *doc){

    std::string sub;
    int uid = route("user", path, sub);

    Account u(uid);
    if(!u) return;

    if(sub == ""){
        doc->setHtml("html/user.tpl", u.name);
        u.fill(doc->dict());
    }

}

}
