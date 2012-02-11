#include "actions.h"
#include "../render/html.h"
#include "../render/http.h"
#include "../account.h"
#include "../session.h"

using namespace Html;

std::string accountForm(const Account &_account, const std::string &error=std::string(),
                                const std::string &message=std::string()){
    return Html::header("Your account") + "<h2>Your account</h2>" +
        (message.empty() ? "" : "<div class=\"message\">" + message + "</div>") +
        (error.empty() ? "" : "<div class=\"error\">" + error + "</div>") +
        "<form action=\"/account\" method=\"post\">"
            "<table>"
                "<tr>"
                    "<td><label for=\"r_name\">Display name:</label></td>"
                    "<td><input id=\"r_name\" name=\"name\" value=\""+escape(_account.name())+"\""
                    " maxlength=\"200\" /></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_email\">Email:</label></td>"
                    "<td><input id=\"r_email\" name=\"email\" value=\""+escape(_account.email())+"\" /></td>"
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
                    "<td><textarea id=\"r_about\" name=\"about\">" + escape(_account.about()) + "</textarea></td>"
                "</tr>"
            "</table>"
            "<input type=\"submit\" value=\"Update\" />"
        "</form>"
        + Html::footer();
}

std::string Action::account(cgicc::Cgicc &cgi){
    if(!Session::user()) return Http::redirect("/login?redirect=/account");
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
        Session::destroy();
        Session::start(cgi);
        std::vector<Track> tracks = Track::byArtist(a.id(), true);
        for(std::vector<Track>::iterator i=tracks.begin(); i!=tracks.end(); i++)
            i->updateTags();
    }
    return accountForm(a, std::string(), "Changes successful.");
}
