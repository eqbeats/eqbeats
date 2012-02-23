#include "actions.h"
#include "../render/html/page.h"
#include "../render/http.h"
#include "../contest.h"
#include "../number.h"
#include "../track.h"
#include "../user.h"
#include "../session.h"

using namespace Render;

void Action::contestSubmission(int cid){
    Contest c(cid);
    if(!c) return Html::notFound("Contest");
    Track t(number(cgi("tid")));
    if(!t) return Html::notFound("Track");
    if(Session::user() == t.artist())
        c.addTrack(t.id());
    Http::redirect(c.url()+"#submissions");
}
