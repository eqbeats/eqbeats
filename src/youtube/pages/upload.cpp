#include "pages.h"
#include "../youtube.h"
#include "../../misc/repl.h"
#include <core/cgi.h>
#include <account/session.h>
#include <core/db.h>
#include <text/text.h>
#include <text/modifiers.h>
#include <track/art.h>

#include <unistd.h>

void Pages::yt_upload(Document *doc){

    std::string sub;
    int tid = route("track", path, sub);

    if(tid > 0 && sub == "youtube_upload"){
        ExtendedTrack t(tid);
        Youtube yt(Session::user().id);

        if(Session::user() != t.artist || !yt)
            return doc->redirect(t.url());

        if(cgi.getEnvironment().getRequestMethod() != "POST" || Session::nonce() != cgi("nonce")){
            doc->setHtml("html/youtube.tpl", "YouTube upload preview");
            doc->dict()->SetValue("ARTIST", Session::user().name);
            doc->dict()->SetValue("TRUNC_TITLE", t.title.substr(0,100));
            Art a(tid);
            if (a) {
                doc->dict()->SetValue("COVER_URL", a.url(Art::Medium));
                doc->dict()->ShowSection("HAS_COVER");
            }
            else doc->dict()->ShowSection("NO_COVER");
            doc->dict()->SetValue("DESCRIPTION", t.notes);
            doc->dict()->SetValue("URL", eqbeatsUrl() + t.url());
            doc->dict()->SetValue("MP3_URL", eqbeatsUrl() + t.url() + "/mp3");
            //doc->dict()->SetValue("MP3_URL", t.url(Track::MP3));

            std::vector<std::string> & tags = t.tags;
            if(!tags.empty()){
                for(std::vector<std::string>::const_iterator i = tags.begin(); i != tags.end(); i++)
                    doc->dict()->SetValueAndShowSection("TAG_NAME", *i, "TAG");
            }
        }
        else {
            Session::newNonce();
            doc->setHtml("html/bsod.tpl", "Uploading video...");
            if (yt.upload(t)){
                doc->dict()->SetValue("TITLE", "Success");
                doc->dict()->SetValue("MESSAGE",
                    "The new video should appear <a href=\"https://www.youtube.com/my_videos\">on "
                    "your YouTube account</a> in the next few seconds.");
            }
            else {
                doc->dict()->SetValue("TITLE", "There was a problem");
                doc->dict()->SetValue("MESSAGE",
                    "Try again in a few minutes. If it still doesn't work, drop us a note.<br/>"
                    "<a href=\""+t.url()+"\">Back to "+t.title+"</a>");
            }
        }
    }
}
