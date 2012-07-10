#include "pages.h"
#include <account/account.h>
#include <account/session.h>
#include <core/cgi.h>
#include <text/text.h>
#include <track/list.h>

void Pages::user(Document *doc){

    std::string sub;
    int uid = route("user", path, sub);

    if(uid && sub == ""){

        Account u(uid);
        if(!u) return;

        doc->setHtml("html/user.tpl", u.name);
        u.fill(doc->dict());
        Tracks::byUser(u.id, Session::user() == u).fill(doc->dict(), "TRACK_LIST");

        Dict *uploader = doc->dict()->AddIncludeDictionary("UPLOADER");
        uploader->SetFilename("html/uploader.tpl");
        uploader->SetValue("ACTION", "/track/new");

    }

}
