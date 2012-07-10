#include "pages.h"
#include "../account.h"
#include "../session.h"
#include <core/cgi.h>
#include <core/db.h>
#include <misc/hash.h>
#include <misc/mail.h>
#include <text/text.h>

static void form(Document *doc, const char *err=""){
    doc->setHtml("html/password-reset.tpl", "Password reset");
    doc->dict()->SetValueAndShowSection("ERROR", err, "ERROR");
}

void Pages::passwordReset(Document *doc){

    if(path != "/account/reset")
        return;

    if(Session::user())
        doc->redirect("/account");

    // Coming from an email

    if(!cgi("token").empty()){
        DB::Result r = DB::query("SELECT user_id FROM resets WHERE token = $1", cgi("token"));

        if(r.empty())
            form(doc, "Sorry, looks like your token has expired. You could always try again.");

        else{
            DB::query("DELETE FROM resets WHERE token = $1", cgi("token"));
            std::string pw = randomString();
            int uid = number(r[0][0]);

            Account a(uid);
            DB::query("UPDATE users SET password = crypt($1, gen_salt('bf')) WHERE id = " + number(a.id), pw);
            std::string sid = Session::login(a);

            doc->setHtml("html/account.tpl", "Your account");
            doc->addHttp("Set-Cookie: sid=" + sid + ";Max-Age=2592000;Path=/\n"); // 30 days
            doc->dict()->SetValueAndShowSection("MESSAGE", "Your password has been reset to " + pw + ". You can now change it below.", "MESSAGE");
            doc->dict()->SetValue("OLD_PASSWORD", pw);
            a.fill(doc->dict());
        }

    }

    // Sending the token

    else if(cgi.getEnvironment().getRequestMethod() == "POST" && !cgi("email").empty()){
        DB::Result r = DB::query("SELECT id FROM users WHERE lower(email) = lower($1)", cgi("email"));

        if(r.empty())
            form(doc, "Sorry, we couldn't find any account with this email address. Try again?");

        else{
            std::string token = randomString() + randomString();
            while(DB::query("INSERT INTO resets (user_id, token) VALUES ($1, $2) RETURNING 1", r[0][0], token).empty())
                token = randomString() + randomString();

            sendMail(cgi("email").c_str(),
                ((std::string)"From: EqBeats <eqbeats@eqbeats.org>\n"
                "Subject: Resetting your password on EqBeats\n\n"
                "Hi.\n\n"
                "Someone, hopefully you, requested a password reset on your account at EqBeats. "
                "If you want to reset your password, please click this link : " + eqbeatsUrl() + "/account/reset?token=" + token + "\n"
                "If you didn't request it, please ignore this email.\n\n"
                "Cheers.").c_str());

            doc->setHtml("html/password-reset.tpl", "Password reset");
            doc->dict()->SetValueAndShowSection("MESSAGE",
                "A reset link has been sent to your email address. "
                "If there's any more problems (can't access your email, etc...), "
                "feel free to drop us a line at contact@eqbeats.org.", "MESSAGE");
        }

    }

    // Not coming from a form nor an email
    else
        form(doc);

}
