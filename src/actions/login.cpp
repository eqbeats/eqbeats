#include "actions.h"
#include "../cgi.h"
#include "../session.h"
#include "../user.h"

void Action::login(){
    // Already logged
    if(Session::user())
        ;//Http::redirect(cgi("redirect").empty()? Session::user().url() : cgi("redirect"));
    // Check
    else if(cgi.getEnvironment().getRequestMethod() != "POST")
        ;//loginForm();
    else if(cgi("email").empty())
        ;//loginForm("Please specify an Email address.");
    else if(cgi("pw").empty())
        ;//loginForm("Please specify a password.");
    // Logging in
    else{
        std::string sid = Session::login(cgi("email"), cgi("pw"));
        if(sid.empty())
            ;//loginForm("Sorry, wrong credentials.");
        else {
            //o << "Set-Cookie: sid=" << sid << ";Max-Age=2592000\n"; // 30 days
            //Http::redirect(cgi("redirect").empty()? Session::user().url() : cgi("redirect"));
            //o << "Logged in, redirecting...";
        }
    }
}

void Action::logout(){
    Session::logout();
    /*o << "Set-Cookie: sid=\n";
    Http::redirect(cgi("redirect").empty()?"/":cgi("redirect"));
    o << "Logged out, redirecting...";*/
}
