#include "pages.h"
#include "../session.h"
#include <core/cgi.h>
#include <core/db.h>
#include <log/log.h>
#include <text/text.h>

static void form(Document *doc, const char *err=""){
    doc->setHtml("html/registration.tpl", "Register");
    doc->dict()->SetValueAndShowSection("ERROR", err, "ERROR");
    doc->dict()->SetValue("NAME", cgi("name"));
    doc->dict()->SetValue("EMAIL", cgi("email"));
}

void Pages::registration(Document *doc){

    if(path != "/register")
        return;

    std::string name = cgi("name"), email = cgi("email"), pw = cgi("pw");

    if(Session::user())
        doc->redirect("/");

    else if(cgi.getEnvironment().getRequestMethod() != "POST")
        form(doc);

    else if(name.empty())
        form(doc, "Please specify a display name.");

    else if(email.empty())
        form(doc, "Please specify an email address.");
    else if(!validEmail(email))
        form(doc, "Invalid email address.");

    else if(pw.empty())
        form(doc, "Please specify a password.");
    else if(pw != cgi("pwconf"))
        form(doc, "Passwords mismatch.");

    else{

        if(DB::query("SELECT EXISTS (SELECT 1 FROM users WHERE lower(name) = lower($1) OR lower(email) = lower($2))", name, email)[0][0] == "t")
            return form(doc, "Sorry, name or email already in use.");

        DB::Result r = DB::query(
            "INSERT INTO users (name, password, email, registration, last_login) "
            "VALUES ($1, crypt($2, gen_salt('bf')), $3, 'now', 'now') "
            "RETURNING id", name, pw, email);

        if(r.empty())
            return form(doc, "Erm, something went wrong. Please try again.");

        User u = User(number(r[0][0]), name);
        log("New user: " + u.name + " (" + number(u.id) + ")");
        doc->addHttp("Set-Cookie: sid=" + Session::login(u) + ";Max-Age=2592000\n"); // 30 days
        doc->redirect(u.url() + "?welcome=1");

    }

}
