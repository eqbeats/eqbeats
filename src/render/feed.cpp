#include "feed.h"
#include "render.h"
#include "http.h"
#include "html/escape.h"
#include "../user.h"
#include "../track.h"
#include "../category.h"
#include "../path.h"
#include <vector>

using namespace Render;
using namespace std;

void notFound(const string what){
    Http::header("application/atom+xml", 404);
    o << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
         "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
            "<title>" << what << " Not Found</title>"
            "<link rel=\"alternate\" href=\"" << eqbeatsUrl() << "\" />"
            "<id>" << eqbeatsUrl() << "/404</id>"
            "<updated>1970-01-01 01:00:00+01:00</updated>"
         "</feed>";
}

void feed(const string &title, const string &link, const vector<Track> &tracks){
    Http::header("application/atom+xml");
    o << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
         "<feed xmlns=\"http://www.w3.org/2005/Atom\">"
           "<title>" << title << " on EqBeats</title>"
           "<link rel=\"alternate\" href=\"" << eqbeatsUrl() << link << "\" />"
           "<id>" << eqbeatsUrl() << link << "</id>"
           "<updated>" << (tracks.empty()? "1970-01-01 01:00:00+01:00": tracks[0].date()) << "</updated>";
    for(std::vector<Track>::const_iterator i=tracks.begin(); i!=tracks.end(); i++){
        string notes = i->getNotes();
        o << "<entry>"
                "<title>" << Html::escape(i->artist()) << " - " << Html::escape(i->title()) << "</title>"
                "<updated>" << i->date() << "</updated>"
                "<id>" << eqbeatsUrl() << i->url() << "</id>"
                "<author>"
                    "<name>" << Html::escape(i->artist()) << "</name>"
                    "<uri>" << eqbeatsUrl() << User::url(i->artistId()) << "</uri>"
                "</author>"
                "<link rel=\"alternate\" href=\"" << eqbeatsUrl() << i->url() << "\" />"
          <<    (notes.empty()?"":"<content type=\"html\">" + Html::escape(Html::format(notes)) + "</content>")
          <<    "<link rel=\"enclosure\" type=\"audio/ogg\"  href=\"" << eqbeatsUrl() << i->url(Track::Vorbis) << "\" />"
                "<link rel=\"enclosure\" type=\"audio/mpeg\" href=\"" << eqbeatsUrl() << i->url(Track::MP3) << "\" />"
             "</entry>";
    }
    o << "</feed>";
}

void Feed::latest(int n){
    feed("Latest tracks", "/tracks/latest", Track::latest(n));
}

void Feed::user(int uid){
    User u(uid);
    u ? feed(u.name(), u.url(), Track::byArtist(uid))
      : notFound("User");
}

void Feed::category(int cid){
    Category c(cid);
    c ? feed(c.name(), c.url(), Track::byCategory(cid))
      : notFound("Category");
}
