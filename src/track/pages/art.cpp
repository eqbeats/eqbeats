#include "pages.h"
#include "../art.h"
#include "../track.h"
#include <core/cgi.h>
#include <text/text.h>

void Pages::art(Document *doc){

    std::string sub;
    int tid = route("track", path, sub);
    if(!tid) return;
    if(sub.substr(0,3) != "art") return;
    Track t(tid);

    Art art(tid);

    // Download
    if(art){
        std::string base = t.artist.name + " - " + t.title;
             if(sub == "art")        doc->download(art.full().setBaseName(base));
        else if(sub == "art/medium") doc->download(art.medium().setBaseName(base + ".medium"));
        else if(sub == "art/thumb")  doc->download(art.thumbnail().setBaseName(base + ".thumb"));
    }

}
