#include "pages.h"
#include <track/extended.h>
#include <core/cgi.h>
#include <core/db.h>
#include <text/text.h>
#include <account/session.h>
#include <account/account.h>

#define TICK(x, y) if(license == x) doc->dict()->ShowSection(y)

void Pages::license(Document* doc){

    std::string sub;
    int tid = route("track", path, sub);
    if(sub != "license" && path != "/account/license") return;

    ExtendedTrack t(tid);
    if(tid && !t) return;
    if(t && !t.artist.self()) return doc->redirect(t.url());

    if(cgi.getEnvironment().getRequestMethod() == "POST" && Session::nonce() == cgi("nonce")){
        Session::newNonce();
        std::string l = cgi("license");
        if(l == "custom") l = cgi("custom-license");
        if(l.empty()) return doc->redirect(t.url() + "/license");

        if(!validString(l))
            return doc->badRequest();

        bool mkdefault = (!t || !cgi("mkdefault").empty()), retro = !cgi("retro").empty();

        if(mkdefault)
            DB::query("UPDATE users SET license = $1 WHERE id = " + number(Session::user().id), l);
        if(retro)
            DB::query("UPDATE tracks SET license = $1 WHERE user_id = " + number(Session::user().id), l);
        else if(t)
            DB::query("UPDATE tracks SET license = $1 WHERE id = " + number(t.id), l);

        if(t) doc->redirect(t.url());
        else doc->redirect("/account");
    } else {
        doc->setHtml("html/license.tpl", "License selection");
        std::string license;
        if(t){
            doc->dict()->ShowSection("TRACK");
            doc->dict()->SetValue("TITLE", t.title);
            doc->dict()->SetValue("URL", t.url());
            license = t.license;
        } else {
            doc->dict()->ShowSection("NO_TRACK");
            license = Account(Session::user().id).license;
        }

        TICK("Copyright", "COPYRIGHT");
        else TICK("CC BY", "CC_BY");
        else TICK("CC BY-NC", "CC_BY_NC");
        else TICK("CC BY-SA", "CC_BY_SA");
        else TICK("CC BY-ND", "CC_BY_ND");
        else TICK("CC BY-NC-SA", "CC_BY_NC_SA");
        else TICK("CC BY-NC-ND", "CC_BY_NC_ND");
        else TICK("Public domain", "PUBLIC");
        else {
            doc->dict()->ShowSection("CUSTOM");
            doc->dict()->SetValue("CUSTOM", license);
        }
    }
}
