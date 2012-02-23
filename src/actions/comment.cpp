#include "actions.h"
#include "../render/html/page.h"
#include "../render/http.h"
#include "../news.h"
#include "../track.h"
#include "../session.h"
#include "../user.h"

using namespace Render;

void Action::postComment(Comment::Type type, int ref){
    if(type == Comment::Track && !Track(ref)) Html::notFound("Track");
    else if(type == Comment::User && !User(ref)) Html::notFound("User");
    else if(type == Comment::News && !News(ref)) Html::notFound("News");

    else {
        if(!cgi("msg").empty() && cgi.getEnvironment().getRequestMethod() == "POST")
            Comment::add(cgi("msg"), cgi("name"), Session::user().id(), ref, type);
        Http::redirect(
            type == Comment::Track ? Track::url(ref) + "#comments" :
            type == Comment::User ? User::url(ref) + "#comments" :
            type == Comment::News ? News::url(ref) + "#comments" : "/"
        );
    }
}
