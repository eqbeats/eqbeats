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

static void deleteAccount(Document *doc){
    if(!Session::user())
        return doc->redirect("/");

    bool post = cgi.getEnvironment().getRequestMethod() == "POST";
    if(!post || cgi("confirm") != "Delete" || Session::nonce() != cgi("nonce")){
        Session::newNonce();
        doc->setHtml("html/delete-account.tpl", "Account deletion");
        doc->dict()->SetValue("CANCEL_URL", "/account");
    }

    else{
        Session::user().deleteAccount();
        doc->redirect("/goodbye");
    }
}

void Pages::account(Document *doc){

    if(path == "/account/delete")
        return deleteAccount(doc);
    else if (path == "/goodbye")
        return doc->setHtml("html/goodbye.tpl", "Goodbye");

    if(path != "/account")
        return;

    if(!Session::user())
        return doc->redirect("/login?redirect=/account");

    Account a(Session::user().id);

    if(cgi.getEnvironment().getRequestMethod() != "POST" || Session::nonce() != cgi("nonce"))
        return form(doc, a);

    Session::newNonce();

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
