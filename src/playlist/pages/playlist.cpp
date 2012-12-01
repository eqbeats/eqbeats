#include "pages.h"
#include "../playlist.h"
#include <core/cgi.h>

void Pages::playlist(Document *doc){
    std::string sub;
    int id = route("playlist", path, sub);

    if(id && (sub == "" || sub == "json")){

        Playlist p(id);
        if(!p) return;

        if(sub == "json"){
            doc->setJson("json/playlist.tpl");
            p.tracks().fillJson(doc->dict());
        } else {
            doc->setHtml("html/playlist.tpl", p.name());
            Dict* list = p.tracks().fill(doc->dict(), "TRACKLIST", p.author().self());
            list->SetIntValue("PLAYLIST_ID", p.id());
        }
        p.fill(doc->dict());
    }

}
