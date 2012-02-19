#include "actions.h"
#include "../render/html.h"
#include "../render/http.h"
#include "../contest.h"
#include "../utils.h"
#include "../session.h"

std::string Action::contestSubmission(int cid){
    Contest c(cid);
    if(!c) return Html::notFound("Contest");
    Track t(number((*cgi)("tid")));
    if(!t) return Html::notFound("Track");
    if(Session::user().id() == t.artistId())
        c.addTrack(t.id());
    return Http::redirect(c.url()+"#submissions");
}
