#include "pages.h"
#include "../youtube.h"
#include "../../misc/repl.h"
#include <core/cgi.h>
#include <account/session.h>
#include <core/db.h>
#include <text/text.h>
#include <unistd.h>

void Pages::oauth(Document *doc){
    if(path != "/oauth/yt" && path != "/oauth/yt/unlink")
        return;

    if(!Session::user())
        return doc->redirect("/login?redirect=/account");

    Youtube yt(Session::user().id);

    if(path == "/oauth/yt"){
        std::string error = cgi("error");
        std::string code = cgi("code");
        if (!error.empty() || code.empty())
            return doc->redirect("/account");

        doc->setHtml("html/bsod.tpl", "Linking your YouTube account");
        if (!code.empty() && yt.link(code)){
            doc->dict()->SetValue("TITLE", "Success");
            doc->dict()->SetValue("MESSAGE", "Your YouTube account is now linked to your EqBeats account.");
        } else {
            doc->dict()->SetValue("TITLE", "There was a problem");
            doc->dict()->SetValue("MESSAGE", "Try again in a few minutes. If it still doesn't work, drop us a note.");
        }
    }
    else if (path == "/oauth/yt/unlink"){
        yt.unlink();
        doc->redirect("/account");
    }
}
