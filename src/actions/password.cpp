#include "actions.h"
#include "../cgi.h"
#include "../session.h"
#include "../account.h"
#include "../render/html/page.h"
#include "../render/http.h"
#include "../render/render.h"
#include "../db.h"
#include "../number.h"
#include "../path.h"
#include "../mail.h"

using namespace Render;

void resetForm(){
    Html::header("Password reset");
    o << "<h2>Reset your password</h2>"
      << "<form action=\"/account/reset\" method=\"post\">"
           "Email: <input name=\"email\" /> <input type=\"submit\" value=\"Reset\" />"
         "</form>";
    Html::footer();
}

void Action::passwordReset(){
    // Already logged
    if(Session::user())
        Http::redirect("/account");
    // Coming from an email
    if(!cgi("token").empty()){
        DB::Result r = DB::query("SELECT user_id FROM resets WHERE token = $1", cgi("token"));
        if(r.empty()){
            Html::header("Password reset");
            o << "Sorry, looks like your token has expired. You could always <a href=\"/account/reset\">try again</a>.";
            Html::footer();
        } else {
            DB::query("DELETE FROM resets WHERE token = $1", cgi("token"));
            std::string pw = Session::randomString();
            int uid = number(r[0][0]);
            Account a(uid);
            a.setPassword(pw);

            std::string sid = Session::login(uid, cgi.getEnvironment().getRemoteAddr());
            o << "Set-Cookie: sid=" << sid << ";Max-Age=2592000;Path=/\n"; // 30 days
            Action::accountForm(a, "", "Your password has been reset to " + pw + ". You can now change it below.", pw);
        }
    }
    // Sending the token
    else if(cgi.getEnvironment().getRequestMethod() == "POST" && !cgi("email").empty()){
        User u = User::withEmail(cgi("email"));
        if(!u){
            Html::header("Password reset");
            o << "Sorry, we couldn't find any account with this email address. <a href=\"/account/reset\">Try again</a>?";
            Html::footer();
            return;
        }
        std::string token = Session::randomString() + Session::randomString();
        DB::Result r = DB::query("INSERT INTO resets (user_id, token) VALUES ($1, $2) RETURNING 1", number(u.id()), token);
        if(r.empty()){ // duplicate token, try again
            passwordReset();
            return;
        }
        sendMail(cgi("email").c_str(),
            ((std::string)"From: EqBeats <eqbeats@eqbeats.org>\n"
            "Subject: Resetting your password on EqBeats\n\n"
            "Hi.\n\n"
            "Someone, hopefully you, requested a password reset on your account at EqBeats. "
            "If you want to reset your password, please click this link : " + eqbeatsUrl() + "/account/reset?token=" + token + "\n"
            "If you didn't request it, please ignore this email.\n\n"
            "Cheers.").c_str());
        Html::header("Password reset");
        o << "<div class=\"message\">A reset link has been sent to your email address.<br/>"
             "If there's any more problems (can't access your email, etc...), feel free to drop us a line at contact@eqbeats.org.</div>";
        Html::footer();
    }
    // Not coming from a form nor an email
    else resetForm();
}
