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
            Dict* tracks = doc->dict()->AddIncludeDictionary("TRACKS");
            tracks->SetFilename("json/array.tpl");
            doc->dict()->ShowSection("LONG");

            p.tracks().fillJson(tracks);
        } else {
            doc->setHtml("html/playlist.tpl", p.name());
            Dict* list = p.tracks().fill(doc->dict(), "TRACKLIST", p.author().self());
            list->SetIntValue("PLAYLIST_ID", p.id());
            if(cgi("firstrun") == "1")
                doc->dict()->ShowSection("FIRSTRUN");
        }
        p.fill(doc->dict());
    }

}
