#include "actions.h"
#include "../account.h"
#include "../session.h"
#include "../track.h"
#include "../media.h"
#include "../log.h"
#include "../cgi.h"

void Action::account(){
    if(!Session::user()) return; // redirect("/login?redirect=/account");
    Account a(Session::user().id());
    bool newName = false;
    if(cgi.getEnvironment().getRequestMethod() != "POST")
        return;
    if(!cgi("name").empty()){
        newName = a.name() != cgi("name");
        if(!a.setName(cgi("name")))
            return; // Name already in use.
    }
    if(!cgi("email").empty()){
        if(!Account::validEmail(cgi("email")))
            return;// Invalid email address.
        if(!a.setEmail(cgi("email")))
            return; // Email already in use.
    }
    if(!cgi("oldpw").empty() && !cgi("newpw").empty()){
        if(cgi("newpw") != cgi("newpwconf"))
            return; // Passwords mismatch.
        if(!a.checkPassword(cgi("oldpw")))
            return; // Wrong password.
        a.setPassword(cgi("newpw"));
    }
    a.setAbout(cgi("about"));
    a.setNotify(!cgi("notify").empty());
    a.commit();
    if(newName){
        log("Renaming user: " + Session::user().name() + " -> " + a.name());
        Session::destroy();
        Session::start();
        std::vector<Track> tracks = a.tracks(true);
        for(std::vector<Track>::iterator i=tracks.begin(); i!=tracks.end(); i++)
            Media(*i).updateTags();
    }
    // Changes successful.
}
