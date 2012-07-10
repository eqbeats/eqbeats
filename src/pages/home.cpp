#include "pages.h"
#include <core/cgi.h>
#include <track/list.h>

void Pages::home(Document *doc){
    if(path != "") return;
    doc->setHtml("html/home.tpl");
    doc->rootDict()->SetValueAndShowSection("FEED_URL", "/tracks/latest/atom", "FEED");
    Tracks::latest(7).fill(doc->dict(), "LATEST_TRACKS");
    Tracks::featured(7).fill(doc->dict(), "FEATURED_TRACKS");
    Tracks::random(3).fill(doc->dict(), "RANDOM_TRACKS");
}
