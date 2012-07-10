#include "pages.h"
#include "../session.h"
#include "../user.h"
#include <core/cgi.h>
#include <render/document.h>

namespace Pages {

void login(Document *doc){

    if(path == "/login"){
        std::string sid;
        if(Session::login(doc->dict(), &sid)){
            if(!sid.empty()) doc->addHttp("Set-Cookie: sid=" + sid + ";Max-Age=2592000\n"); // 30 days
            doc->redirect(cgi("redirect").empty() ? Session::user().url() : cgi("redirect"));
        }
        else
            doc->setHtml("html/login.tpl", "Login");
    }

    else if(path == "/logout"){
        Session::logout();
        doc->addHttp("Set-Cookie: sid=\n");
        doc->redirect(cgi("redirect").empty() ? "/" : cgi("redirect"));
    }

}

}
