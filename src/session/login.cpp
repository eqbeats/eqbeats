#include "session.h"
#include <account/user.h>
#include <core/cgi.h>

#define ERROR(e) { \
    d->SetValueAndShowSection("ERROR", (e), "HAS_ERROR"); \
    d->SetValue("EMAIL", cgi("email")); \
    return false; \
}

bool Session::login(Dict *d, std::string *sid){

    // Already logged
    if(Session::user())
        return true;

    // Coming from elsewhere
    if(cgi.getEnvironment().getRequestMethod() != "POST")
        return false;

    // Missing field
    if(cgi("email").empty())
        ERROR("Please specify an Email address.")
    if(cgi("pw").empty())
        ERROR("Please specify a password.")

    // Logging in
    *sid = Session::login(cgi("email"), cgi("pw"));
    if(sid->empty())
        ERROR("Sorry, wrong credentials.")
    else
        return true;

}
