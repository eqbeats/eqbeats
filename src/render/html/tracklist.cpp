#include "track.h"
#include "page.h"
#include "escape.h"
#include "feed.h"
#include "forms.h"
#include "../render.h"
#include "../../track.h"
#include "../../flag.h"
#include "../../user.h"
#include "../../number.h"
#include "../../cgi.h"

using namespace Render;
using namespace std;

void Html::trackList(const vector<Track> &tracks, Html::TrackList l){
    if(tracks.empty()){ o << "<div class=\"empty tracklist\">Nothing here yet.</div>"; return; }
    o << "<ul class=\"tracklist\">";
    for(vector<Track>::const_iterator i=tracks.begin(); i!=tracks.end(); i++){
        o << "<li";
        if(!i->visible()) o << " class=\"hidden\"";
        o << "><a href=\"" << i->url() << "\">" << escape(i->title()) << "</a>";
        if(l != Compact)
            o << " <span class=\"by\">by <a href=\"" << i->artist().url() << "\">"
              << escape(i->artist().name()) << "</a></span>";
        o << "</li>";
    }
    o << "</ul>";
}

void Html::tracksPage(const string &title, const vector<Track> &tracks){
    header(title);
    o << "<h2>" << title << "</h2>";
    trackList(tracks);
    footer();
}

void Html::latestTracks(unsigned n){
    header("Latest tracks", atomFeed("/tracks/latest/atom"));
    o << "<h2>Latest tracks ";
    feedIcon("/tracks/latest/atom");
    o << "</h2>";
    int p = number(cgi("p"));
    if(p < 1) p = 1;
    std::vector<Track> tracks = Track::latest(n+1, n*(p-1));
    bool lastPage = tracks.size() < n+1;
    if(!lastPage) tracks.pop_back();
    trackList(tracks);
    if(p != 1) o << "<a href=\"?p=" << p-1 << "\">&laquo; Newer tracks</a>";
    if(p != 1 && !lastPage) o << " - ";
    if(!lastPage) o << "<a href=\"?p=" << p+1 << "\">Older tracks &raquo;</a>";
    footer();
}

void Html::trackSearch(const std::string &q){
    header("Track search");
    o << "<h2>Track search</h2>";
    searchForm("/tracks/search", q);
    if(!q.empty()){
        vector<Track> res = Track::search(q);
        if(res.empty())
            o << "No result.";
        else
            trackList(res);
    }
    footer();
}

void Html::category(int cid){
    Flag c(cid);
    if(!c || c.type() != Flag::Category) return notFound("Category");
    header(c.name(), atomFeed(c.url()+"/atom"));
    o << "<h2>" << c.name() << "</h2>";
    trackList(c.tracks());
    footer();
}

