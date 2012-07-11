#include "pages.h"
#include "../follower.h"
#include <account/user.h>
#include <core/cgi.h>
#include <track/list.h>

void Pages::favorites(Document *doc){
    std::string sub;
    int uid = route("user", path, sub);
    if(!uid) return;

    if(sub == "favorites"){
        User u(uid);
        if(!u) return;
        doc->setHtml("html/tracklist-page.tpl", u.name + " - Favorite tracks");
        doc->dict()->SetValue("TITLE", u.name + "'s favorite tracks");
        Follower(uid).favorites().fill(doc->dict(), "TRACKLIST");
    }

    else if(sub == "favorites/json"){
        User u(uid);
        if(!u) return;
        TrackList tracks = Follower(u.id).favorites();
        doc->setJson("json/array.tpl");
        Dict *item, *data;
        for(TrackList::const_iterator i=tracks.begin(); i!=tracks.end(); i++){
            item = doc->dict()->AddSectionDictionary("ITEM");
            data = item->AddIncludeDictionary("DATA");
            data->SetFilename("json/track.tpl");
            i->fill(data);
        }
    }

}
