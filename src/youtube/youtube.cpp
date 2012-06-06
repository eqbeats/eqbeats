#include "actions.h"
#include "../repl.h"
#include "../track.h"
#include "../session.h"
#include "../path.h"
#include <text/text.h>
#include "../cgi.h"
#include "../db.h"
#include "../art.h"
#include "../log.h"
#include <unistd.h>

void Action::youtubeUpload(int tid){
    ExtendedTrack t(tid);
    if(Session::user() != t.artist() || !Session::user().hasYoutube()) return; // Http::redirect(t.url());
    if(cgi.getEnvironment().getRequestMethod() != "POST"){
        //Html::header("YouTube upload preview"); ...
    } else {
        Repl ytmgr(("/tmp/ytmgr-"+number(getuid())+".sock").c_str());
        //Html::header("YouTube upload");
        if(ytmgr.exec("upload "+number(tid)).find("OK") != 0){
            /*o << "<h2>There was a problem</h2>"
                 "Try again in a few minutes. If it still doesn't work, drop us a note.<br/>"
                 "<a href=\""+t.url()+"\">Back to "+Html::escape(t.title())+"</a>";*/
        } else {
            /*o << "<h2>Upload started</h2>"
                 "The new video should appear <a href=\"https://www.youtube.com/my_videos\">on your YouTube account</a> in the next few seconds.";*/
        }
    }
}

void Action::youtubeOauthCallback(){
    if(!Session::user()) return; // Http::redirect("/login?redirect=/account");
    if(!cgi("error").empty() || cgi("code").empty()) return; // Http::redirect("/account");
    Repl ytmgr(("/tmp/ytmgr-"+number(getuid())+".sock").c_str());
    //Html::header("Linking your YouTube account");
    if(ytmgr.exec("auth " + cgi("code") + " " + number(Session::user().id())).find("OK") != 0){
        /*o << "<h2>There was a problem</h2>"
             "Try again in a few minutes. If it still doesn't work, drop us a note.";*/
    } else {
        /*o << "<h2>Success</h2>"
             "Your YouTube account is now linked to your EqBeats account.";*/
    }
}

void Action::youtubeUnlink(){
    DB::query("DELETE FROM youtube_refresh_tokens WHERE user_id = "+number(Session::user().id()));
    //Http::redirect("/account");
}
