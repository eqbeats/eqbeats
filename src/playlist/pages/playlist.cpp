#include "pages.h"
#include "../playlist.h"
#include <core/cgi.h>

void Pages::playlist(Document *doc){
    std::string sub;
    int id = route("playlist", path, sub);

    if(id && sub == ""){

        Playlist p(id);
        if(!p) return;

        doc->setHtml("html/playlist.tpl", p.name());
        p.fill(doc->dict());
        Dict *list = p.tracks().fill(doc->dict(), "TRACKLIST", p.author().self());
        list->SetIntValue("PLAYLIST_ID", p.id());

    }

}
