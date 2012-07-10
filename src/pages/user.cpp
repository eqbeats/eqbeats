#include "pages.h"
#include <account/account.h>
#include <account/session.h>
#include <core/cgi.h>
#include <playlist/playlist.h>
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

        std::vector<Playlist> playlists = Playlist::forUser(u);
        doc->dict()->ShowSection(playlists.empty() ? "NO_PLAYLIST" : "HAS_PLAYLISTS");
        for(std::vector<Playlist>::const_iterator i=playlists.begin(); i!=playlists.end(); i++){
            Dict *playlistDict = doc->dict()->AddSectionDictionary("PLAYLIST");
            i->fill(playlistDict);
        }

    }

}
