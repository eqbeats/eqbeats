#include "html.h"
#include "../track.h"
#include "../user.h"
#include "../utils.h"

using namespace std;
std::string notFound(const string what){
    return "Status: 404 Not Found\n"
    "Content-Type: application/atom+xml; charset=UTF-8\n\n"
    "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
    "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title>" + what + " Not Found</title>"
        "<link rel=\"alternate\" href=\"" + eqbeatsUrl() + "\" />"
        "<id>" + eqbeatsUrl() + "/404</id>"
        "<updated>1970-01-01 01:00:00+01:00</updated>"
    "</feed>";
}
std::string feed(const string &title, const string &link, const vector<Track> &tracks){
    std::string atom =
    "Status: 200 OK\n"
    "Content-Type: application/atom+xml; charset=UTF-8\n\n"
    "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
    "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
        "<title>" + title + " on EqBeats</title>"
        "<link rel=\"alternate\" href=\"" + eqbeatsUrl() + link + "\" />"
        "<id>" + eqbeatsUrl() + link + "</id>"
        "<updated>" + (tracks.empty()? "1970-01-01 01:00:00+01:00": tracks[0].date()) + "</updated>";
    for(std::vector<Track>::const_iterator i = tracks.begin(); i != tracks.end(); i++) {
        User u(i->artistId());
        atom +=
            "<entry>"
                "<title>" + Html::escape(u.name()) + " - " + Html::escape(i->title()) + "</title>"
                "<updated>" + i->date() + "</updated>"
                "<id>" + eqbeatsUrl() + i->url() + "</id>"
                "<author>"
                    "<name>" + Html::escape(u.name()) + "</name>"
                    "<uri>" + eqbeatsUrl() + u.url() + "</uri>"
                "</author>"
                "<link rel=\"alternate\" href=\"" + eqbeatsUrl() + i->url() + "\" />" +
                (i->getNotes().empty()?"":"<content type=\"html\">" + Html::escape(Html::format(i->getNotes())) + "</content>") +
                "<link rel=\"enclosure\" type=\"audio/ogg\"  href=\"" + eqbeatsUrl() +i->url(Track::Vorbis)+"\" />"
                "<link rel=\"enclosure\" type=\"audio/mpeg\" href=\"" + eqbeatsUrl() +i->url(Track::MP3)+"\" />"
            "</entry>";
    }
    atom +="</feed>";
    return atom;
}

std::string Html::tracksFeed(int n){
    return feed("Latest tracks", "/tracks/latest", Track::latest(n));
}
std::string Html::userFeed(int uid){
    User u(uid);
    if (!u) return ::notFound("User");
    return feed(u.name(), u.url(), Track::byArtist(uid));
}

std::string Html::categoryFeed(int cid){
    Category c(cid);
    if (!c) return ::notFound("Category");
    return feed(c.name(), c.url(), Track::byCategory(cid));
}

std::string Html::feedIcon(const std::string &url){
    return "<a class=\"feed\" title=\"Feed\" href=\"" + url + "\"><span>(Feed)</span></a>";
}
