#include <stdio.h>
#include "actions.h"
#include "../render/html.h"
#include "../render/http.h"
#include "../session.h"
#include "../utils.h"
#include "../mail.h"

using namespace std;

std::string filter(const std::string &str){
    std::string buf;
    for(std::string::const_iterator i = str.begin(); i!=str.end(); i++){
        if(*i != '\n' && *i != '\r') buf += *i;
    }
    return buf;
}

std::string Action::publishTrack(int tid, cgicc::Cgicc &cgi){
    if(tid != number(cgi("tid")))
        return Http::redirect(Track::url(tid));
    User u = Session::user();
    Track t(tid);
    if(u.id() == t.artistId() && t && !t.visible() && u &&
        cgi.getEnvironment().getRequestMethod() == "POST"){
        t.setVisible(true);
        t.bump();
        // Mail
        std::vector<std::string> emails = u.followers();
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
            sendMail(i->c_str(), maildata.c_str());
    }
    return Http::redirect(t.url());
}

std::string Action::updateNotes(int tid, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(tid);
    if(u.id()==t.artistId() && u &&
       cgi.getEnvironment().getRequestMethod() == "POST" )
        t.setNotes(cgi("notes"));
    return Http::redirect(t.url());
}

std::string Action::renameTrack(int tid, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(tid);
    if(u.id()==t.artistId() && u && !cgi("title").empty() &&
       cgi.getEnvironment().getRequestMethod() == "POST" )
        t.setTitle(cgi("title"));
    return Http::redirect(t.url());
}

std::string deletionForm(const Track &t){
    return Html::header("Track deletion") +
        "<form method=\"post\">"
            "Do you really want to delete <b>" + Html::escape(t.title()) + "</b> ? "
            "<input type=\"submit\" value=\"Delete\" name=\"confirm\" />"
        "</form>"
        "<a class=\"danger\" href=\"" + t.url() + "\">Cancel</a>"
        + Html::footer();
}

std::string Action::deleteTrack(int tid, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(tid);
    if(u.id()!=t.artistId() || !u)
        return Http::redirect(t.url());
    if(cgi.getEnvironment().getRequestMethod()!="POST" || cgi("confirm")!="Delete")
        return deletionForm(t);
    t.remove();
    return Http::redirect(u.url());
}

std::string Action::updateCategories(int tid, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(tid);
    if(u.id()!=t.artistId() || !u || cgi.getEnvironment().getRequestMethod()!="POST")
        return Http::redirect(t.url());
    
    if(!cgi("rmcats").empty()){
        vector<Category> cats = t.getCategories();
        vector<int> ids;
        for(vector<Category>::iterator i = cats.begin(); i != cats.end(); i++)
            if(!cgi(number(i->id())).empty())
                ids.push_back(i->id());
        if(!ids.empty())
            t.removeCategories(ids);
    }
    else if(!cgi("cat").empty())
        t.addCategory(number(cgi("cat")));
    return Http::redirect(t.url());
}
