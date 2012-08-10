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
#include "../db.h"
#include "../art.h"
#include "../log.h"
#include <unistd.h>

using namespace Render;

void Action::youtubeUpload(int tid){
    ExtendedTrack t(tid);
    if(Session::user() != t.artist() || !Session::user().hasYoutube()) return Http::redirect(t.url());
    if(cgi.getEnvironment().getRequestMethod() != "POST"){
        Html::header("YouTube upload preview");
        o << "<div class=\"ytpreview\">"
             "<h2>" << (Html::escape(Session::user().name()) + " - " + Html::escape(t.title())).substr(0,100) << "</h2>";
        Art a(tid);
        o << "<div class=\"ytplayer\">"
                 "<img src=\"" << (a?a.url(Art::Medium):"/static/placeholder.jpg") << "\"/>"
                 "<div class=\"overlay\"></div>"
             "</div>"
             "<p>" << Html::format(t.notes())
          << "<br/> --<br/><a href=\""<< t.url() <<"\">"<< eqbeatsUrl() << t.url() <<"</a><br/>Download: <a href=\""<<t.url(Track::MP3)<<"\">"<<eqbeatsUrl()<<t.url(Track::MP3)<<"</a></p>"
             "<div class=\"tags\">Tags :<br/>";
        std::vector<std::string> tags = t.tags();
        int tagslen = 0;
        for(std::vector<std::string>::iterator tag=tags.begin(); tag != tags.end(); tag++){
            o << "<span class=\"tag\">" << Html::escape(*tag) << "</span> ";
            tagslen += tag->size() + 2;
        }
        o << "</div>"
             "</div>"
             "<div class=\"ytsidebar\">"
             "This is what your video will look like. If you have cover art, make sure you upload it before you continue.";
        if (tagslen >= 500) {
            o << "<div class=\"message error\">There are a lot of tags, some of them may be lost during upload</div>";
        }
        o << "<form method=\"POST\"><input type=\"submit\" value=\"Continue\"/> &nbsp; <a class=\"danger\" href=\""<< t.url() <<"\">Cancel</a></form>"
             "</div>"
             "<div style=\"clear: both\"></div>";
        Html::footer();
    } else {
        Repl ytmgr(("/tmp/ytmgr-"+number(getuid())+".sock").c_str());
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
}

void Action::youtubeOauthCallback(){
    if(!Session::user()) return Http::redirect("/login?redirect=/account");
    if(!cgi("error").empty() || cgi("code").empty()) return Http::redirect("/account");
    Repl ytmgr(("/tmp/ytmgr-"+number(getuid())+".sock").c_str());
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

void Action::youtubeUnlink(){
    DB::query("DELETE FROM youtube_refresh_tokens WHERE user_id = "+number(Session::user().id()));
    Http::redirect("/account");
}
