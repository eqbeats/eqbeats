#include "actions.h"
#include "../cgi.h"
#include "../track.h"
#include "../session.h"

void Action::setLicense(int tid){
    User u = Session::user();
    if(!u) return; // Http::redirect("/");
    ExtendedTrack t(tid);
    std::string l = cgi("license");
    if(t && t.artist() != u)
        ;//Http::redirect(t.url());
    else if(cgi.getEnvironment().getRequestMethod() != "POST" || l.empty()); // display form
    else{
        if(l == "custom") l = cgi("custom-license");
        if(!l.empty()){
            if(t) t.setLicense(l);
            bool mkdefault = (!t || !cgi("mkdefault").empty()), retro = !cgi("retro").empty();
            if(mkdefault || retro)
                Account(u.id()).setLicense(l, mkdefault, retro);
        }
        //Http::redirect(t ? t.url() : "/account");
    }
}
