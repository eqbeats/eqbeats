#include "pages.h"
#include <core/cgi.h>
#include <track/list.h>

void Pages::statics(Document *doc){
    if(path == "/quickstart")
        doc->setHtml("html/quickstart.tpl", "Thanks");
    else if(path == "/faq")
        doc->setHtml("html/faq.tpl", "FAQ");
    else if(path == "/credits")
        doc->setHtml("html/credits.tpl", "Credits");
    else if(path == "/api")
        doc->setHtml("html/api.tpl", "API");
}

void Pages::home(Document *doc){
    if(path != "") return;
    doc->setHtml("html/home.tpl");
    doc->rootDict()->SetValueAndShowSection("FEED_URL", "/tracks/latest/atom", "FEED");
    Tracks::latest(7).fill(doc->dict(), "LATEST_TRACKS");
    Tracks::featured(7).fill(doc->dict(), "FEATURED_TRACKS");
    Tracks::random(3).fill(doc->dict(), "RANDOM_TRACKS");
}
