#include "actions.h"
#include "../session.h"
#include "../render/html.h"
#include "../render/http.h"

std::string loginForm(const std::string &error=std::string()){
    return Html::header("Login") + "<h2>Login</h2>" +
        (error.empty() ? "" : "<div class=\"error\">" + error + "</div>") +
        "<form action=\"/login\" method=\"post\">"
            "<table>"
                "<tr>"
                    "<td><label for=\"r_email\">Email:</label></td>"
                    "<td><input id=\"r_email\" name=\"email\" "
                    "value=\""+(*Action::cgi)("email")+"\"/></td>"
                "</tr>"
                "<tr>"
                    "<td><label for=\"r_pw\">Password:</label></td>"
                    "<td><input id=\"r_pw\" name=\"pw\" type=\"password\" /></td>"
                "</tr>"
            "</table>"
            "<input type=\"submit\" value=\"Login\" />"
            "<input type=\"hidden\" name=\"redirect\" value=\""+(*Action::cgi)("redirect")+"\" />"
        "</form>"
        "<a href=\"/register\">Register</a>"
        + Html::footer();
}

std::string Action::login(){
    if(Session::user()) return Http::redirect((*cgi)("redirect").empty()? Session::user().url() : (*cgi)("redirect"));

    if(cgi->getEnvironment().getRequestMethod() != "POST")
        return loginForm();

    if((*cgi)("email").empty())
        return loginForm("Please specify an Email address.");

    if((*cgi)("pw").empty())
        return loginForm("Please specify a password.");

    std::string sid = Session::login((*cgi)("email"), (*cgi)("pw"), cgi->getEnvironment().getRemoteAddr());
    if(sid.empty())
        return loginForm("Sorry, wrong credentials.");

    return "Set-Cookie: sid=" + sid + ";Max-Age=2592000\n" // 30 days
    + Http::redirect((*cgi)("redirect").empty()? Session::user().url() : (*cgi)("redirect")) +
        "Logged in, redirecting...";
}

std::string Action::logout(){
    Session::logout();
    return "Set-Cookie: sid=\n" + Http::redirect((*cgi)("redirect").empty()?"/":(*cgi)("redirect")) +
        "Logged out, redirecting...";
}
