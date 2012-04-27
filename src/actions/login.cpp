#include "actions.h"
#include "../cgi.h"
#include "../session.h"
#include "../user.h"
#include "../render/html/page.h"
#include "../render/http.h"
#include "../render/render.h"
#include "../render/html/escape.h"

using namespace Render;

void loginForm(const std::string &error=std::string()){
    Html::header("Login");
    o << "<h2>Login</h2>"
      << (error.empty() ? "" : "<div class=\"error\">" + error + "</div>")
      << "<form action=\"/login\" method=\"post\">"
            "<table>"
                "<tr>"
                    "<td><label for=\"r_email\">Email:</label></td>"
                    "<td><input id=\"r_email\" name=\"email\" "
                    "value=\"" << cgi("email") << "\"/></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_pw\">Password:</label></td>"
                    "<td><input id=\"r_pw\" name=\"pw\" type=\"password\" /></td>"
                "</tr>"
            "</table>"
            "<input type=\"submit\" value=\"Login\" />"
            "<input type=\"hidden\" name=\"redirect\" value=\"" << Html::escape(cgi("redirect")) << "\" />"
        "</form>"
        "<a href=\"/register\">Register</a> &middot; <a href=\"/account/reset\">Lost your passord?</a>";
    Html::footer();
}

void Action::login(){
    // Already logged
    if(Session::user())
        Http::redirect(cgi("redirect").empty()? Session::user().url() : cgi("redirect"));
    // Check
    else if(cgi.getEnvironment().getRequestMethod() != "POST")
        loginForm();
    else if(cgi("email").empty())
        loginForm("Please specify an Email address.");
    else if(cgi("pw").empty())
        loginForm("Please specify a password.");
    // Logging in
    else{
        std::string sid = Session::login(cgi("email"), cgi("pw"));
        if(sid.empty())
            loginForm("Sorry, wrong credentials.");
        else {
            o << "Set-Cookie: sid=" << sid << ";Max-Age=2592000\n"; // 30 days
            Http::redirect(cgi("redirect").empty()? Session::user().url() : cgi("redirect"));
            o << "Logged in, redirecting...";
        }
    }
}

void Action::logout(){
    Session::logout();
    o << "Set-Cookie: sid=\n";
    Http::redirect(cgi("redirect").empty()?"/":cgi("redirect"));
    o << "Logged out, redirecting...";
}
