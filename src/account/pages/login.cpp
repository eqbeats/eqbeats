#include "pages.h"
#include "../session.h"
#include <core/cgi.h>

using namespace std;

static void form(Document *doc, const char *err=""){
    doc->setHtml("html/login.tpl", "Login");
    doc->dict()->SetValueAndShowSection("ERROR", err, "ERROR");
    doc->dict()->SetValue("EMAIL", cgi("email"));
}

static void redir(Document *doc){
    doc->redirect(cgi("redirect").empty() || cgi("redirect")[0] != '/' ? Session::user().url() : cgi("redirect"));
}

void Pages::login(Document *doc){

    if(path == "/login"){

        if(Session::user())
            return redir(doc);

        if(cgi.getEnvironment().getRequestMethod() != "POST")
            return form(doc);

        if(cgi("email").empty())
            return form(doc, "Please specify an email address.");
        if(cgi("pw").empty())
            return form(doc, "Please specify a password.");

        if(cgi("email").find('@') == string::npos || cgi("email").find('.') == string::npos)
            return form(doc, "That doesn't look like an email address. Make sure you enter your email, not your screenname.");

        std::string sid = Session::login(cgi("email"), cgi("pw"));
        if(sid.empty())
            return form(doc, "Sorry, wrong credentials.");

        doc->addHttp("Set-Cookie: sid=" + sid + ";Max-Age=2592000\n"); // 30 days
        redir(doc);

    }

    else if(path == "/logout"){
        Session::logout();
        doc->addHttp("Set-Cookie: sid=\n");
        doc->redirect(cgi("redirect").empty() || cgi("redirect")[0] != '/' ? "/" : cgi("redirect"));
    }
}
void Pages::JSONLogin(Document *doc){
    if(path == "/login/json"){
        if(cgi.getEnvironment().getRequestMethod() == "POST"){
            if (Session::login(cgi("email"), cgi("pw")).empty()){
                doc->setJson("json/error.tpl", 403);
                doc->dict()->SetValue("ERROR", "Invalid credentials.");
            }
            else {
                doc->setJson("json/login.tpl");
                Session::fill(doc->dict());
            }
        } else {
            doc->setJson("json/error.tpl", 405);
            doc->dict()->SetValue("ERROR", "This resource can only be accessed with POST.");
        }
    }

    else if(path == "/logout/json"){
        if(cgi.getEnvironment().getRequestMethod() == "POST"){
            if(Session::logout())
                doc->setJson("json/ok.tpl");
            else {
                doc->setJson("json/error.tpl", 403);
                doc->dict()->SetValue("ERROR", "Not logged in.");
            }
        } else {
            doc->setJson("json/error.tpl", 405);
            doc->dict()->SetValue("ERROR", "This resource can only be accessed with POST.");
        }
    }
}
