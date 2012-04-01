#include "actions.h"
#include "../render/html/page.h"
#include "../render/http.h"
#include "../news.h"
#include "../track.h"
#include "../session.h"
#include "../user.h"
#include "../cgi.h"
#include "../event.h"

using namespace Render;

void Action::postComment(Comment::Type type, int ref){
    Track ref_t;
    User ref_u;
    News ref_n;
    if(type == Comment::Track)     ref_t = Track(ref);
    else if(type == Comment::User) ref_u = User(ref);
    else if(type == Comment::News) ref_n = News(ref);

    if(type == Comment::Track && !ref_t) Html::notFound("Track");
    else if(type == Comment::User && !ref_u) Html::notFound("User");
    else if(type == Comment::News && !ref_n) Html::notFound("News");

    else {
        if(!cgi("msg").empty() && cgi.getEnvironment().getRequestMethod() == "POST"){
            User u = Session::user() ? Session::user() : User(0, cgi("name"));
            if(type == Comment::Track)
                Event::comment(Track(ref), u, cgi("msg"));
            else if(type == Comment::User)
                Event::comment(User(ref), u, cgi("msg"));
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
