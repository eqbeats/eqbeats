#include "pages.h"
#include <account/account.h>
#include <account/session.h>
#include <core/cgi.h>
#include <playlist/playlist.h>
#include <social/event.h>
#include <social/follower.h>
#include <text/text.h>
#include <track/list.h>
#include <stat/stat.h>
#include <userfeature/feature.h>

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

        Stat::push("userView", uid);

        Dict *uploader = doc->dict()->AddIncludeDictionary("UPLOADER");
        uploader->SetFilename("html/uploader.tpl");
        uploader->SetValue("ACTION", "/track/new");

        std::vector<Playlist> playlists = Playlist::forUser(u);
        doc->dict()->ShowSection(playlists.empty() ? "NO_PLAYLIST" : "HAS_PLAYLISTS");
        for(std::vector<Playlist>::const_iterator i=playlists.begin(); i!=playlists.end(); i++){
            Dict *playlistDict = doc->dict()->AddSectionDictionary("PLAYLIST");
            i->fill(playlistDict);
        }

        Session::fill(doc->dict());
        EventList::user(u, 12).fill(doc->dict(), "EVENTS", true);
        Follower(u.id).followed().fill(doc->dict(), "FOLLOWED_USERS");
        if(!u.self())
            doc->dict()->ShowSection(Follower(Session::user().id).following(u.id) ? "IS_FOLLOWED" : "NOT_FOLLOWED");
        if(!cgi("welcome").empty())
            doc->dict()->ShowSection("WELCOME");

        Feature(u.id).fill(doc->dict());
    }
}
