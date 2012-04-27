#include "track.h"
#include "page.h"
#include "escape.h"
#include "feed.h"
#include "forms.h"
#include "player.h"
#include "../render.h"
#include "../../track.h"
#include "../../user.h"
#include "../../art.h"
#include "../../number.h"
#include "../../cgi.h"

using namespace Render;
using namespace std;

void Html::trackListEntry(const Track t, std::string list, std::string extraButtons){
    Art art(t.id());
    o << "<li"<< (t.visible()?"":" class=\"hidden\"") << " onclick=\"toggle(this.getElementsByClassName('player')[0]);\">";
    o << extraButtons;
    if(art) o << "<img class=\"cover\" alt=\"\" src=\"" << art.url(Art::Thumbnail) << "\" />";
    o << "<div class=\"title\"><a href=\"" << t.url() << "\" title=\""<< escape(t.title()) <<"\">" << escape(t.title()) << "</a></div>"
         " <div class=\"by\">by <a href=\"" << t.artist().url() << "\">"
                << escape(t.artist().name()) << "</a></div>";
    o << "<div style=\"clear:both;\"></div>";
    player(t, list, false);
    o << "</li>";
}
void Html::trackList(const vector<Track> &tracks, const std::string &list){
    if(tracks.empty()){ o << "<div class=\"empty tracklist\">Nothing here yet.</div>"; return; }
    o << "<ul class=\"tracklist\">";
    for(vector<Track>::const_iterator i=tracks.begin(); i!=tracks.end(); i++)
        trackListEntry(*i, list);
    o << "</ul>";
}

void Html::tracksPage(const string &title, const vector<Track> &tracks){
    header(title);
    o << "<h2>" << escape(title) << "</h2>";
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
