#include "actions.h"
#include "../render/html.h"
#include "../render/http.h"
#include "../news.h"
#include "../session.h"

std::string Action::postComment(Comment::Type type, int ref){
    if(type == Comment::Track && !Track(ref)) return Html::notFound("Track");
    if(type == Comment::User && !User(ref)) return Html::notFound("User");
    if(type == Comment::News && !News(ref)) return Html::notFound("News");

    if(!(*cgi)("msg").empty() && cgi->getEnvironment().getRequestMethod() == "POST")
        Comment::add((*cgi)("msg"), (*cgi)("name"), Session::user().id(), ref, type);
    return Http::redirect(
        type == Comment::Track ? Track::url(ref) + "#comments" :
        type == Comment::User ? User::url(ref) + "#comments" :
        type == Comment::News ? News::url(ref) + "#comments" : "/"
    );
}
