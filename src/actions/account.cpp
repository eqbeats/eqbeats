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

void Action::accountForm(const Account &_account, const std::string &error,
                                const std::string &message, const std::string &oldpw){
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
                    "<td><input type=\"password\" id=\"r_oldpw\" name=\"oldpw\" value=\"" << oldpw << "\" /></td>"
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
                    "<td><label for=\"r_notify\">Email notifications:</label></td>"
                    "<td><input id=\"r_notify\" name=\"notify\" type=\"checkbox\" " << (_account.notify()?"checked=\"checked\" ":"") << "/></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_about\">Description:</label><br />"
                    "<span class=\"legend\">(tags: [b]old, [u]nderline, [i]talic)</span>"
                    "<td><textarea id=\"r_about\" name=\"about\">" << escape(_account.about()) << "</textarea></td>"
                "</tr>"
            "</table>"
            "<input type=\"submit\" value=\"Update\" />"
        "</form>";

    if(_account.hasYoutube()){
    Render::o << "<form action=\"/oauth/yt/unlink\">"
                 "Your YouTube account is linked."
                 " <a class=\"huh\" href=\"/faq#youtube\">Huh?</a><br/>"
                 "<input type=\"submit\" value=\"Unlink your youtube account\"/>"
                 "</form>";
    } else {
//https://accounts.google.com/o/oauth2/auth?response_type=code&client_id=767490682254.apps.googleusercontent.com&approval_prompt=force&scope=https%3A%2F%2Fuploads.gdata.youtube.com%2Ffeeds%2Fapi%2Fusers%2Fdefault%2Fuploads&access_type=offline&redirect_uri=http%3A%2F%2Feqbeats.org%2Foauth%2Fyt";
    Render::o << "<form action=\"https://accounts.google.com/o/oauth2/auth\">"
                 "Your YouTube account is not linked."
                 " <a class=\"huh\" href=\"/faq#youtube\">Huh?</a><br/>"
                 "<input type=\"submit\" value=\"Link your youtube account\"/>"
                 "<input type=\"hidden\" name=\"response_type\" value=\"code\"/>"
                 "<input type=\"hidden\" name=\"client_id\" value=\"767490682254.apps.googleusercontent.com\"/>"
                 "<input type=\"hidden\" name=\"scope\" value=\"https://uploads.gdata.youtube.com/feeds/api/users/default/uploads\"/>"
                 "<input type=\"hidden\" name=\"access_type\" value=\"offline\"/>"
                 "<input type=\"hidden\" name=\"redirect_uri\" value=\"http://eqbeats.org/oauth/yt\"/>"
                 "<input type=\"hidden\" name=\"approval_prompt\" value=\"force\"/>"
                 "</form>";
    }
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
    a.setNotify(!cgi("notify").empty());
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
