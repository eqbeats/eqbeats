#include "pages.h"
#include "../audio.h"
#include <account/session.h>
#include <core/cgi.h>
#include <core/db.h>
#include <log/log.h>
#include <text/text.h>

std::string filter(const std::string &str){
    std::string buf;
    for(std::string::const_iterator i = str.begin(); i!=str.end(); i++){
        if(*i != '\n' && *i != '\r') buf += *i;
    }
    return buf;
}

void Pages::trackActions(Document *doc){
    std::string sub;
    int tid = route("track", path, sub);
    if(!tid) return;
    bool post = cgi.getEnvironment().getRequestMethod() == "POST";

    if(sub == "rename"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self()){
            std::string title = cgi("title");
            if(!title.empty() && title != t.title){
                DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(t.id), title);
                t.title = title;
                Audio(&t).updateTags();
            }
        }
        doc->redirect(t.url());
    }

    else if(sub == "tags"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self())
            DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(t.id), cgi("tags"));
        doc->redirect(t.url());
    }

    else if(sub == "notes"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self())
            DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(t.id), cgi("notes"));
        doc->redirect(t.url());
    }

    else if(sub == "flags"){
        Track t(tid);
        if(!t) return;
        if(post && t.artist.self())
            DB::query("UPDATE tracks SET airable = $1 WHERE id = " + number(t.id), cgi.queryCheckbox("airable") ? "t" : "f");
        doc->redirect(t.url());
    }

    else if(sub == "report"){
        Track t(tid);
        if(!t) return;
        if(post){
            std::string path = eqbeatsDir() + "/reports";
            std::ofstream f(path.c_str(), std::ios_base::app);
            f << t.artist.id << " " << t.artist.name << " - " << t.id << " " << t.title << std::endl;
            f.close();
        }
        doc->redirect(t.url());
    }

    else if(sub == "delete"){
        Track t(tid);
        if(!t) return;
        if(!t.artist.self())
            doc->redirect(t.url());
        else if(!post || cgi("confirm") != "Delete"){
            doc->setHtml("html/delete.tpl", "Track deletion");
            doc->dict()->SetValue("WHAT", t.title);
        }
        else{
            log("Deleting track: " + t.title + " (" + number(t.id) + ")");
            //Art art(tid);
            //if(art) art.remove();
            //Media(t).unlink();
            //t.remove();
            doc->redirect(Session::user().url());
        }
    }

    else if(sub == "publish"){
        Track t(tid);
        if(!t) return;
        if(tid != number(cgi("tid")))
            return doc->redirect(t.url());

        if(t.artist.self() && !t.visible && post){

            DB::query("UPDATE tracks SET visible = 't', date = 'now' WHERE id = " + number(t.id));
            //Event::publish(t);

            // Mail
            /*std::vector<std::string> emails = Follower(u).followers();
            std::string maildata =
                "From: EqBeats notification <notify@eqbeats.org>\n"
                "Message-ID: notify-t" + number(t.id()) + "\n"
                "Subject: " + filter("EqBeats notification: " + u.name() + " - " + t.title()) + "\n"
                "Precedence: bulk\n\n" +
                u.name() + " just published a new track : " + t.title() + "\n"
                "Listen to it here : " + eqbeatsUrl() + t.url() + "\n\n"
                "You're receiving this email because you're following " + u.name() + " on EqBeats.\n"
                "If you don't want to receive these notifications anymore, go to " + eqbeatsUrl() + u.url() + " and click \"Stop following\".";
            for(std::vector<std::string>::const_iterator i = emails.begin(); i!=emails.end(); i++)
                sendMail(i->c_str(), maildata.c_str());*/

        }
        doc->redirect(t.url());

    }

}
