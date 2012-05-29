#include "actions.h"
#include "../cgi.h"
#include "../track.h"
#include "../session.h"
#include "../render/render.h"
#include "../render/http.h"
#include "../render/html/page.h"
#include "../render/html/escape.h"

using namespace Render;

#define CHOICE(l,i) (std::string) \
    "<input name=\"license\" type=\"radio\" value=\"" + l + "\" id=\"" + i + "\" " + \
    (t.license() == l ? found = true, "checked=\"checked\" ": "") + "/>"

void licenseForm(const ExtendedTrack &t){
    Html::header("License selection");
    bool found = false;
    o << "<h2>" << (t ? "License for <a href=\"" + t.url() + "\">" + Html::escape(t.title()) + "</a>" : "Global license") << "</h2>"
         "<form method=\"post\"><table class=\"licenses\">"
            "<tr>"
                "<td>" << CHOICE("Copyright", "lc") << "</td>"
                "<td>"
                    "<b><label for=\"lc\">Copyright</label></b>"
                    "<div>Default license. Most restrictive. If you don't plan on selling your music, we recommand that you use the Creative Commons license below.</div>"
                "</td>"
            "</tr>";
    o <<    "<tr>"
                "<td>" << CHOICE("CC BY-NC", "lnc") << "</td>"
                "<td>"
                    "<b><label for=\"lnc\">Creative Commons: Attribution-NonCommercial (CC BY-NC)</label></b>"
                    "<div>"
                        "&ldquo;This license lets others remix, tweak, and build upon your work non-commercially, and although their new works must also acknowledge you and be non-commercial, they don’t have to license their derivative works on the same terms.&rdquo;<br />"
                        "<a href=\"http://creativecommons.org/licenses/\">More about the Creative Commons licenses.</a><br /><br />"
                        "Other CC licenses: ";
    o <<                CHOICE ("CC BY", "lby") << " <label for=\"lby\">BY</label> ";
    o <<                CHOICE ("CC BY-SA", "lsa") << " <label for=\"lsa\">BY-SA</label> ";
    o <<                CHOICE ("CC BY-ND", "lnd") << " <label for=\"lnd\">BY-ND</label> ";
    o <<                CHOICE ("CC BY-NC-SA", "lncsa") << " <label for=\"lncsa\">BY-NC-SA</label> ";
    o <<                CHOICE ("CC BY-NC-ND", "lncnd") << " <label for=\"lncnd\">BY-NC-ND</label>"
                "</td>"
            "</tr>";
    o <<    "<tr>"
                "<td>" << CHOICE("Public domain", "lpub") << "</td>"
                "<td>"
                    "<b><label for=\"lpub\">Public Domain</label></b>"
                    "<div>Very permissive. Use that if you make your music for fun and don't care about what people do with it.</div>"
                "</td>"
            "</tr>";
    o <<    "<tr>"
                "<td><input name=\"license\" type=\"radio\" value=\"custom\" id=\"lcus\" " << (found?"":"checked=\"checked\" ") << "/></td>"
                "<td><b><label for=\"lcus\">Custom:</label> </b><input name=\"custom-license\" " << (found?"":"value=\""+Html::escape(t.license())+"\" ") << "/></td>"
            "</tr>"
            << (t ? "<tr><td><input name=\"mkdefault\" type=\"checkbox\" id=\"ldef\" /></td><td><label for=\"ldef\">Make it the default license.</label></td></tr>" : "") <<
            "<tr><td><input name=\"retro\" type=\"checkbox\" id=\"lall\"/></td><td><label for=\"lall\">Apply to all tracks.</label></td></tr>"
            "<tr><td colspan=\"2\"><input type=\"submit\" value=\"Update\" /></td></tr>"
         "</table></form>";
    Html::footer();
}

void Action::setLicense(int tid){
    User u = Session::user();
    if(!u) return Http::redirect("/");
    ExtendedTrack t(tid);
    std::string l = cgi("license");
    if(t && t.artist() != u)
        Http::redirect(t.url());
    else if(cgi.getEnvironment().getRequestMethod() != "POST" || l.empty())
        licenseForm(t);
    else{
        if(l == "custom") l = cgi("custom-license");
        if(!l.empty()){
            if(t) t.setLicense(l);
            bool mkdefault = (!t || !cgi("mkdefault").empty()), retro = !cgi("retro").empty();
            if(mkdefault || retro)
                Account(u.id()).setLicense(l, mkdefault, retro);
        }
        Http::redirect(t ? t.url() : "/account");
    }
}
