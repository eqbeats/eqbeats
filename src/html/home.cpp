#include "html.h"
#include "../session.h"
#include <sstream>

std::string Html::home(){
    std::string hello, log;
    std::stringstream s;
    s << header("Home");
    if(Session::user())
        s << "Hello " << escape(Session::user().name()) << ".<br />";
    s << "<ul>"
            "<li><a href=\"/users\">Users</a></li>"
            "<li><a href=\"/artists\">Artists</a></li>"
        "</ul>";
    if(Session::user())
        s << "<a href=\"" << Session::user().pageUrl() << "\">Your page</a><br />"
             "<a href=\"/account\">Edit account</a><br />"
             "<a href=\"/logout\">Logout</a>";
    else
        s << "<a href=\"/register\">Register</a><br />"
             "<a href=\"/login\">Login</a>";
    s << footer();
    return s.str();
}
