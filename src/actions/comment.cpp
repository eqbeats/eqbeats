#include "actions.h"
#include "../render/html/page.h"
#include "../render/http.h"
#include "../path.h"
#include "../news.h"
#include "../track.h"
#include "../session.h"
#include "../user.h"
#include "../cgi.h"
#include "../event.h"
#include "../mail.h"

using namespace Render;

void Action::postComment(Comment::Type type, int ref){
    Track ref_t;
    Account ref_u;
    News ref_n;
    if(type == Comment::Track)     ref_t = Track(ref);
    else if(type == Comment::User) ref_u = Account(ref);
    else if(type == Comment::News) ref_n = News(ref);

    bool bot = true;
    cgicc::form_iterator url = cgi.getElement("url");
    if(url != cgi.getElements().end())
        bot = !url->getValue().empty();

    if(type == Comment::Track && !ref_t) Html::notFound("Track");
    else if(type == Comment::User && !ref_u) Html::notFound("User");
    else if(type == Comment::News && !ref_n) Html::notFound("News");

    else {
        if(!cgi("msg").empty() && !bot && cgi.getEnvironment().getRequestMethod() == "POST"){
            User u = Session::user() ? Session::user() : User(0, cgi("name"));
            Event e;
            if(type == Comment::Track){
                e = Event::comment(ref_t, u, cgi("msg"));
                ref_u = Account(ref_t.artist().id());
                if(u != ref_u && ref_u.notify()){
                    std::string maildata =
                        (std::string)
                        "From: EqBeats notification <notify@eqbeats.org>\n"
                        "Subject: " + "EqBeats comment notification\n"
                        "Precedence: bulk\n\n" +
                        e.source().name() + " posted a comment on " + ref_t.title() + ":\n\n" +
                        e.message() + "\n\n"
                        "You can view it and answer here : " + eqbeatsUrl() + ref_t.url() + "\n"
                        "If you do not wish to recieve those notifications anymore, you can disable them here : " + eqbeatsUrl() + "/account";
                    sendMail(ref_u.email().c_str(), maildata.c_str());
                }
            }
            else if(type == Comment::User){
                e = Event::comment(ref_u, u, cgi("msg"));
                if(u != ref_u && ref_u.notify()){
                    std::string maildata =
                        (std::string)
                        "From: EqBeats notification <notify@eqbeats.org>\n"
                        "Subject: " + "EqBeats comment notification\n"
                        "Precedence: bulk\n\n" +
                        e.source().name() + " posted a comment on your user page:\n\n" +
                        e.message() + "\n\n"
                        "You can view it and answer here : " + eqbeatsUrl() + ref_u.url() + "\n"
                        "If you do not wish to recieve those notifications anymore, you can disable them here : " + eqbeatsUrl() + "/account";
                    sendMail(ref_u.email().c_str(), maildata.c_str());
                }
            }
            else
                Comment::add(cgi("msg"), u, ref, type);
        }
        Http::redirect(
            type == Comment::Track ? ref_t.url() + "#comments" :
            type == Comment::User  ? ref_u.url() + "#comments" :
            type == Comment::News  ? ref_n.url() + "#comments" : "/"
        );
    }
}
