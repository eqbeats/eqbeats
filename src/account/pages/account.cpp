#include "pages.h"
#include "../account.h"
#include "../session.h"
#include <core/cgi.h>
#include <core/db.h>
#include <log/log.h>
#include <text/text.h>

static void form(Document *doc, const Account &a, const char *err=""){
    doc->setHtml("html/account.tpl", "Your account");
    doc->dict()->SetValueAndShowSection("ERROR", err, "ERROR");
    a.fill(doc->dict());
}

void Pages::account(Document *doc){

    if(path != "/account")
        return;

    if(!Session::user())
        return doc->redirect("/login?redirect=/account");

    Account a(Session::user().id);

    if(cgi.getEnvironment().getRequestMethod() != "POST")
        return form(doc, a);

    std::string name = cgi("name");
    if(!name.empty() && name != a.name){
        if(DB::query("SELECT EXISTS (SELECT 1 FROM users WHERE lower(name) = lower($1))", name)[0][0] == "t")
            return form(doc, a, "Name already in use.");
    }
    else
        name = a.name;

    std::string email = cgi("email");
    if(!email.empty() && email != a.email){
        if(!validEmail(email))
            return form(doc, a, "Invalid email address.");
        if(DB::query("SELECT EXISTS (SELECT 1 FROM users WHERE lower(email) = lower($1))", email)[0][0] == "t")
            return form(doc, a, "Email already in use.");
    }
    else
        email = a.email;

    std::string oldpw = cgi("oldpw"), newpw = cgi("newpw");
    if(!oldpw.empty() && !newpw.empty()){
        if(newpw != cgi("newpwconf"))
            return form(doc, a, "Passwords mismatch.");
        if(DB::query("SELECT EXISTS (SELECT 1 FROM users WHERE id = " + number(a.id) + " AND password = crypt($1, password))", oldpw)[0][0] != "t")
            return form(doc, a, "Wrong password.");

        // Everything has been checked. Committing.

        DB::query("UPDATE users SET password = crypt($1, gen_salt('bf')) WHERE id = " + number(a.id), newpw);
    }


    DB::query(
        "UPDATE users SET name = $1, email = $2, about = $3, notify = $4 WHERE id = " + number(a.id),
        name, email, cgi("about"), cgi("notify").empty() ? "f" : "t"
    );

    if(a.name != cgi("name")){ // The name has changed.
        log("Renaming user: " + a.name + " -> " + cgi("name"));
        Session::destroy();
        Session::start();
        //std::vector<Track> tracks = a.tracks(true);
        //for(std::vector<Track>::iterator i=tracks.begin(); i!=tracks.end(); i++)
            //Media(*i).updateTags();
    }

    doc->setHtml("html/account.tpl", "Your account");
    doc->dict()->SetValueAndShowSection("MESSAGE", "Changes applied.", "MESSAGE");
    Account(a.id).fill(doc->dict());

}
