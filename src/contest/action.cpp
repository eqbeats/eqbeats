#include "actions.h"
#include "../contest.h"
#include "../number.h"
#include "../track.h"
#include "../user.h"
#include "../session.h"
#include "../cgi.h"

void Action::contestSubmission(int cid){
    Contest c(cid);
    if(!c);
    Track t(number(cgi("tid")));
    if(!t);
    if(Session::user() == t.artist())
        c.addTrack(t.id());
    //redirect(c.url()+"#submissions");
}
