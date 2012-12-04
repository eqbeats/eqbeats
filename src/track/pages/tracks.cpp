#include "pages.h"
#include "../list.h"
#include <core/cgi.h>
#include <text/text.h>

void Pages::tracks(Document *doc){

    if(path == "/tracks")
        doc->redirect("/");

    // Queries
    else if(path == "/tracks/search"){
        std::string q = cgi("q");
        doc->setHtml("html/tracklist-page.tpl", q);
        doc->dict()->SetValue("TITLE", q);
        doc->dict()->SetValueAndShowSection("SEARCH", q, "SEARCH");
        TrackList::search(q).fill(doc->dict(), "TRACKLIST");
    }
    else if(path.substr(0,12) == "/tracks/tag/"){
        std::string tag = path.substr(12);
        doc->setHtml("html/tracklist-page.tpl", tag);
        doc->dict()->SetValue("TITLE", tag);
        TrackList::tag(tag).fill(doc->dict(), "TRACKLIST");
    }

    // Lists
    else if(path == "/tracks/latest"){
        doc->setHtml("html/tracklist-page.tpl", "Latest tracks");
        doc->dict()->SetValue("TITLE", "Latest tracks");
        doc->rootDict()->SetValueAndShowSection("FEED_URL", "/tracks/latest/atom", "FEED");
        doc->dict()->SetValueAndShowSection("FEED_URL", "/tracks/latest/atom", "FEED");
        int p = number(cgi("p"));
        if(p < 1) p = 1;
        TrackList l = Tracks::latest(16, 15*(p-1));
        if(p > 1)
            doc->dict()->SetValueAndShowSection("PREV", number(p-1), "HAS_PREV");
        if(l.size() == 16){
            doc->dict()->SetValueAndShowSection("NEXT", number(p+1), "HAS_NEXT");
            l.pop_back();
        }
        l.fill(doc->dict(), "TRACKLIST");
    }
    else if(path == "/tracks/random"){
        doc->setHtml("html/tracklist-page.tpl", "Random tracks");
        doc->dict()->SetValue("TITLE", "Random tracks");
        Tracks::random(15).fill(doc->dict(), "TRACKLIST");
    }
    else if(path == "/tracks/featured"){
        doc->setHtml("html/tracklist-page.tpl", "Featured tracks");
        doc->dict()->SetValue("TITLE", "Featured tracks");
        doc->rootDict()->SetValueAndShowSection("FEED_URL", "/tracks/featured/atom", "FEED");
        doc->dict()->SetValueAndShowSection("FEED_URL", "/tracks/featured/atom", "FEED");
        Tracks::featured(15).fill(doc->dict(), "TRACKLIST");
    }

    // JSON
    TrackList json_tracks;
    bool is_json_array = false;
    if(path == "/tracks/search/json")
        is_json_array = true, json_tracks = TrackList::search(cgi("q"));
    if(path == "/tracks/search/exact/json")
        is_json_array = true, json_tracks = TrackList::exactSearch(cgi("artist"), cgi("track"));
    if(path == "/tracks/latest/json")
        is_json_array = true, json_tracks = Tracks::latest(50);
    if(path == "/tracks/random/json")
        is_json_array = true, json_tracks = Tracks::random(50);
    if(path == "/tracks/featured/json")
        is_json_array = true, json_tracks = Tracks::featured(50);
    if(is_json_array){
        doc->setJson("json/array.tpl");
        json_tracks.fillJson(doc->dict());
    }

    // Feeds
    if(path == "/tracks/latest/atom"){
        doc->setTemplate("atom-feed.tpl", "application/atom+xml");
        doc->dict()->SetValue("WHAT", "Latest tracks");
        doc->dict()->SetValue("FEED_URL", "/tracks/latest/atom");
        TrackList l = Tracks::latest(50);
        if(l.empty()) doc->dict()->SetValue("UPDATED", "1970-01-01 01:00:00+00");
        else doc->dict()->SetValue("UPDATED", l[0].date);
        for(TrackList::const_iterator i=l.begin(); i!=l.end(); i++)
            i->fill(doc->dict()->AddSectionDictionary("TRACK"));
    }
    if(path == "/tracks/featured/atom"){
        doc->setTemplate("atom-feed.tpl", "application/atom+xml");
        doc->dict()->SetValue("WHAT", "Featured tracks");
        doc->dict()->SetValue("FEED_URL", "/tracks/featured/atom");
        TrackList l = Tracks::featured(50);
        // Last updated: "today at midnight"
        time_t raw = time(NULL);
        struct tm *now = gmtime(&raw);
        char now_str[128];
        strftime(now_str, 128, "%F 01:00:00+00", now);
        doc->dict()->SetValue("UPDATED", now_str);
        // Entries
        for(TrackList::const_iterator i=l.begin(); i!=l.end(); i++)
            i->fill(doc->dict()->AddSectionDictionary("TRACK"));
    }
}
