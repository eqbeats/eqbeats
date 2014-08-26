#include "pages.h"
#include "../event.h"
#include <account/account.h>
#include <account/session.h>
#include <core/cgi.h>
#include <misc/mail.h>

void Pages::comment(Document *doc){

    std::string sub, _sub;
    int tid = route("track", path, sub);
    int uid = route("user", path, _sub);

    if((tid && sub == "comment") || (uid && _sub == "comment")){

        Track t(tid);
        User u(uid);
        if(!t && !u) return;

        bool bot = true;
        cgicc::form_iterator url = cgi.getElement("url");
        if(url != cgi.getElements().end())
            bot = !url->getValue().empty();

        std::string msg = cgi("msg");
        if(msg.find("<a") != std::string::npos || msg.find("[url") != std::string::npos)
            bot = true;

        if(cgi.getEnvironment().getRequestMethod() != "POST" || msg.empty() || bot || (Session::user() && Session::nonce() != cgi("nonce")))
            return doc->redirect((t ? t.url() : u.url()) + "?spam=spam");

        Session::newNonce();

        Event e;
        e.type = Event::Comment;
        e.source = Session::user() ? Session::user() : User(0, cgi("name"));
        e.target = u ? u : t.artist;
        e.track = t;
        e.message = msg;
        e.push();

        Account dst = Account(e.target.id);
        if(dst != e.source && dst.notify){
            std::string mail = (std::string)
                "From: EqBeats notification <notify@eqbeats.org>\n"
                "Subject: EqBeats comment notification\n"
                "Precedence: bulk\n\n" +
                e.source.name + " posted a comment on " + (t ? t.title : "your user page") + ":\n\n" +
                e.message + "\n\n"
                "You can view it and answer here : " + eqbeatsUrl() + (t ? t.url() : u.url()) + "\n"
                "If you do not wish to recieve those notifications anymore, you can disable them here : " + eqbeatsUrl() + "/account";
            sendMail(dst.email.c_str(), mail.c_str());
        }

        doc->redirect(t ? t.url() : u.url());

    }

}
