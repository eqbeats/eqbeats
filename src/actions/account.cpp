#include "actions.h"
#include "../render/html/page.h"
#include "../render/html/escape.h"
#include "../render/http.h"
#include "../render/render.h"
#include "../render/http.h"
#include "../account.h"
#include "../session.h"
#include "../track.h"
#include "../media.h"
#include "../log.h"
#include "../cgi.h"

using namespace Render::Html;

void accountForm(const Account &_account, const std::string &error=std::string(),
                                const std::string &message=std::string()){
    header("Your account");
    Render::o << "<h2>Your account</h2>" <<
        (message.empty() ? "" : "<div class=\"message\">" + message + "</div>") <<
        (error.empty() ? "" : "<div class=\"error\">" + error + "</div>") <<
        "<form action=\"/account\" method=\"post\">"
            "<table>"
                "<tr>"
                    "<td><label for=\"r_name\">Display name:</label></td>"
                    "<td><input id=\"r_name\" name=\"name\" value=\"" << escape(_account.name()) << "\""
                    " maxlength=\"200\" /></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_email\">Email:</label></td>"
                    "<td><input id=\"r_email\" name=\"email\" value=\"" << escape(_account.email()) << "\" /></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_oldpw\">Old password:</label></td>"
                    "<td><input type=\"password\" id=\"r_oldpw\" name=\"oldpw\" /></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_newpw\">New password:</label></td>"
                    "<td><input type=\"password\" id=\"r_newpw\" name=\"newpw\" /></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_newpwconf\">Confirm new password:</label></td>"
                    "<td><input type=\"password\" id=\"r_newpwconf\" name=\"newpwconf\" /></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_about\">Description:</label></td>"
                    "<td><textarea id=\"r_about\" name=\"about\">" << escape(_account.about()) << "</textarea></td>"
                "</tr>"
            "</table>"
            "<input type=\"submit\" value=\"Update\" />"
        "</form>";
    footer();
}

void Action::account(){
    if(!Session::user()) return Render::Http::redirect("/login?redirect=/account");
    Account a(Session::user().id());
    bool newName = false;
    if(cgi.getEnvironment().getRequestMethod() != "POST")
        return accountForm(a);
    if(!cgi("name").empty()){
        newName = a.name() != cgi("name");
        if(!a.setName(cgi("name")))
            return accountForm(a, "Name already in use.");
    }
    if(!cgi("email").empty()){
        if(!Account::validEmail(cgi("email")))
            return accountForm(a, "Invalid email address.");
        if(!a.setEmail(cgi("email")))
            return accountForm(a, "Email already in use.");
    }
    if(!cgi("oldpw").empty() && !cgi("newpw").empty()){
        if(cgi("newpw") != cgi("newpwconf"))
            return accountForm(a, "Passwords mismatch.");
        if(!a.checkPassword(cgi("oldpw")))
            return accountForm(a, "Wrong password.");
        a.setPassword(cgi("newpw"));
    }
    a.setAbout(cgi("about"));
    a.commit();
    if(newName){
        log("Renaming user: " + Session::user().name() + " -> " + a.name());
        Session::destroy();
        Session::start();
        std::vector<Track> tracks = a.tracks(true);
        for(std::vector<Track>::iterator i=tracks.begin(); i!=tracks.end(); i++)
            Media(*i).updateTags();
    }
    accountForm(a, std::string(), "Changes successful.");
}
