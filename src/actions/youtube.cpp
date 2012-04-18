#include "actions.h"
#include "../render/render.h"
#include "../render/http.h"
#include "../render/html/page.h"
#include "../render/html/escape.h"
#include "../repl.h"
#include "../track.h"
#include "../session.h"
#include "../path.h"
#include "../number.h"
#include "../cgi.h"
#include "../log.h"

using namespace Render;

void Action::youtubeUpload(int tid){
    Track t(tid);
    if(Session::user().hasYoutube()) log("user hasYoutube");
    if(t.artist().hasYoutube()) log("artist hasYoutube");
    if(Session::user() != t.artist() || !Session::user().hasYoutube()) return Http::redirect(t.url());
    Repl ytmgr((eqbeatsDir() + "/ytmgr.sock").c_str());
    Html::header("YouTube upload");
    if(ytmgr.exec("upload "+number(tid)).find("OK") != 0){
        o << "<h2>There was a problem</h2>"
             "Try again in a few minutes. If it still doesn't work, drop us a note.<br/>"
             "<a href=\""+t.url()+"\">Back to "+Html::escape(t.title())+"</a>";
    } else {
        o << "<h2>Upload started</h2>"
             "The new video should appear <a href=\"https://www.youtube.com/my_videos\">on your YouTube account</a> in the next few seconds.";
    }
    Html::footer();
}

void Action::youtubeOauthCallback(){
    if(!Session::user()) return Http::redirect("/login?redirect=/account");
    if(!cgi("error").empty() || cgi("code").empty()) return Http::redirect("/account");
    Repl ytmgr((eqbeatsDir() + "/ytmgr.sock").c_str());
    Html::header("Linking your YouTube account");
    if(ytmgr.exec("auth " + cgi("code") + " " + number(Session::user().id())).find("OK") != 0){
        o << "<h2>There was a problem</h2>"
             "Try again in a few minutes. If it still doesn't work, drop us a note.";
    } else {
        o << "<h2>Success</h2>"
             "Your YouTube account is now linked to your EqBeats account.";
    }
    Html::footer();
}
