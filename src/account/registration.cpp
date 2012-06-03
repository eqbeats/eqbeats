#include "actions.h"
#include "../account.h"
#include "../session.h"
#include "../log.h"
#include "../number.h"
#include "../cgi.h"

void Action::registration(){
    if(Session::user()) return; // Http::redirect("/");
    if(cgi.getEnvironment().getRequestMethod() != "POST")
        return; // registrationForm();
    if(cgi("name").empty())
        return; // registrationForm("Please specify a display name.");
    if(cgi("email").empty())
        return; // registrationForm("Please specify an email address.");
    if(!Account::validEmail(cgi("email")))
        return; // registrationForm("Invalid email address.");
    if(cgi("pw").empty())
        ; //registrationForm("Please specify a password.");
    if(cgi("pw")!=cgi("pwconf"))
        return; // registrationForm("Passwords mismatch.");
    Account account = Account::create(cgi("name"), cgi("pw"), cgi("email"));
    if(!account)
        return; // registrationForm("Sorry, name or email already in use.");

    log("New user: " + account.name() + " (" + number(account.id()) + ")");

    /*o << "Set-Cookie: sid="
      << Session::login(account.id(), cgi.getEnvironment().getRemoteAddr())
      << ";Max-Age=2592000\n"; // 30 days
    Http::redirect("/quickstart");
    o << "Account created, redirecting...";*/
}
