#include "pages.h"
#include <account/account.h>
#include <core/cgi.h>
#include <playlist/playlist.h>
#include <social/event.h>
#include <social/follower.h>
#include <text/text.h>
#include <track/list.h>
#include <userfeature/feature.h>

#ifdef HAVE_LIBHIREDIS
#  include <stat/stat.h>
#endif

void Pages::user(Document *doc){

    std::string sub;
    int uid = route("user", path, sub);

    if(!uid || (sub != "" && sub != "json")) return;

    Account u(uid);
    if(!u) return;

    if(sub == "json"){
        doc->setJson("json/account.tpl");
        u.fill(doc->dict());
        doc->dict()->ShowSection("LONG");
        Tracks::byUser(u.id, 0).fillJson(doc->dict(), false);

        std::vector<Playlist> playlists = Playlist::forUser(u);
        for(std::vector<Playlist>::const_iterator i=playlists.begin(); i!=playlists.end(); i++){
            Dict *playlistDict = doc->dict()->AddSectionDictionary("PLAYLIST");
            Dict *item = playlistDict->AddIncludeDictionary("PLAYLIST");
            item->SetFilename("json/playlist.tpl");
            i->fill(item);
        }

    } else { // plain HTML
        doc->setHtml("html/user.tpl", u.name);
        u.fill(doc->dict());
        Tracks::byUser(u.id, u.self()).fill(doc->dict(), "TRACK_LIST");

#ifdef HAVE_LIBHIREDIS
        Stat::push("userView", uid);
#endif

        std::vector<Playlist> playlists = Playlist::forUser(u);
        doc->dict()->ShowSection(playlists.empty() ? "NO_PLAYLIST" : "HAS_PLAYLISTS");
        for(std::vector<Playlist>::const_iterator i=playlists.begin(); i!=playlists.end(); i++){
            Dict *playlistDict = doc->dict()->AddSectionDictionary("PLAYLIST");
            i->fill(playlistDict);
        }

        EventList::user(u, 12).fill(doc->dict(), "EVENTS");
        Follower(u.id).followed().fill(doc->dict(), "FOLLOWED_USERS");

        Feature(u.id).fill(doc->dict());

    }
}
