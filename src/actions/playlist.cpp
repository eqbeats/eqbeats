#include "actions.h"
#include "../cgi.h"
#include "../number.h"
#include "../playlist.h"
#include "../log.h"
#include "../session.h"
#include "../render/render.h"
#include "../render/http.h"
#include "../render/html/escape.h"
#include "../render/html/page.h"

using namespace Render;

void Action::createPlaylist(){
    if(!Session::user()) return Http::redirect("/");
    std::string name = cgi("name");
    if(name.empty() || cgi.getEnvironment().getRequestMethod() != "POST")
        return Http::redirect(Session::user().url());
    Playlist p = Playlist::create(name);
    log("New playlist: " + p.name() + " (" + number(p.id()) + ")");
    return Http::redirect(p.url());
}

void Action::deletePlaylist(int pid){
    User u = Session::user();
    Playlist p(pid);
    if(u!=p.author() || !u)
        Http::redirect(p.url());
    else if(cgi.getEnvironment().getRequestMethod()!="POST" || cgi("confirm")!="Delete"){
        Html::header("Playlist deletion");
        o << "<form method=\"post\">"
                "Do you really want to delete <b>" << Html::escape(p.name()) << "</b> ? "
                "<input type=\"submit\" value=\"Delete\" name=\"confirm\" />"
            "</form>"
            "<a class=\"danger\" href=\"" << p.url() << "\">Cancel</a>";
        Html::footer();
    }
    else{
        log("Deleting playlist: " + p.name() + " (" + number(p.id()) + ")");
        p.drop();
        Http::redirect(u.url());
    }
}

void Action::editPlaylist(int pid){
    Playlist p(pid);
    if(!p) return Http::redirect("/");
    if(cgi.getEnvironment().getRequestMethod() != "POST") return Http::redirect(p.url());
    if(p.author() != Session::user()) return Http::redirect(p.url());
    std::string nName = cgi("name"), nDesc = cgi("desc");
    p.setMeta(nName, nDesc);
    Http::redirect(p.url());
}

void playlistTrack(int pid, int n, bool add){
    Playlist p(pid);
    if(!p) return Http::redirect("/");
    if(cgi.getEnvironment().getRequestMethod() != "POST" || Session::user() != p.author())
        return Http::redirect(p.url());
    if(add){
        Track t(n);
        if(!t || !t.visible())
            return Http::redirect(p.url());
    }
    add ? p.add(n) : p.remove(n);
    Http::redirect(p.url());
}

void Action::playlistAdd(int tid){ playlistTrack(number(cgi("playlist")), tid, true); }
void Action::playlistRemove(int pid){ playlistTrack(pid, number(cgi("item")), false); }

void Action::playlistMove(int pid){
    Playlist p(pid);
    if(!p) return Http::redirect("/");
    std::string dir = cgi("dir");
    if(cgi.getEnvironment().getRequestMethod() != "POST" || Session::user() != p.author() || (dir != "up" && dir != "down"))
        return Http::redirect(p.url());
    int i = number(cgi("item"));
    p.swap(i, i + (dir == "up" ? -1 : 1));
    Http::redirect(p.url() + "#tracks");
}
