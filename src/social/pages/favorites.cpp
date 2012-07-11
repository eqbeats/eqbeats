#include "pages.h"
#include "../follower.h"
#include <account/user.h>
#include <core/cgi.h>
#include <track/list.h>

void Pages::favorites(Document *doc){

    std::string sub;
    int uid = route("user", path, sub);

    if(!uid || sub != "favorites") return;

    User u(uid);
    if(!u) return;

    doc->setHtml("html/tracklist-page.tpl", u.name + " - Favorite tracks");
    doc->dict()->SetValue("TITLE", u.name + "'s favorite tracks");
    Follower(uid).favorites().fill(doc->dict(), "TRACKLIST");

}
