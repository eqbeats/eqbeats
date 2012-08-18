#include "news.h"
#include "http.h"

using namespace Render;

void Http::newsRedirect(int nid){
    switch(nid){
        case 1: Http::redirect("http://blog.eqbeats.org/ready-set-go"); break;
        case 2: Http::redirect("http://blog.eqbeats.org/already-100-tracks"); break;
        case 3: Http::redirect("http://blog.eqbeats.org/updates/new-design"); break;
        case 4: Http::redirect("http://blog.eqbeats.org/updates/follow-and-share"); break;
        case 6: Http::redirect("http://blog.eqbeats.org/downtime"); break;
        case 7: Http::redirect("http://blog.eqbeats.org/updates/celestia-radio"); break;
        case 8: Http::redirect("http://blog.eqbeats.org/updates/categories"); break;
        case 5: Http::redirect("http://blog.eqbeats.org/wro"); break;
        case 9: Http::redirect("http://blog.eqbeats.org/pon3-mix"); break;
        case 10: Http::redirect("http://blog.eqbeats.org/irc"); break;
        case 11: Http::redirect("http://blog.eqbeats.org/updates/api"); break;
        case 12: Http::redirect("http://blog.eqbeats.org/updates/playlists"); break;
        case 13: Http::redirect("http://blog.eqbeats.org/updates/youtube"); break;
        case 14: Http::redirect("http://blog.eqbeats.org/updates/avatars-and-notifications"); break;
        case 15: Http::redirect("http://blog.eqbeats.org/updates/4-months"); break;
        case 16: Http::redirect("http://blog.eqbeats.org/upcoming-features"); break;
        case 17: Http::redirect("http://blog.eqbeats.org/updates/licenses-and-formats"); break;
        case 18: Http::redirect("http://blog.eqbeats.org/updates/not-dead"); break;
        default: Http::redirect("http://blog.eqbeats.org/");
    }
}
