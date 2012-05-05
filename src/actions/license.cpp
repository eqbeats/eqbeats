#include "actions.h"
#include "../cgi.h"
#include "../track.h"
#include "../session.h"
#include "../render/render.h"
#include "../render/http.h"
#include "../render/html/page.h"
#include "../render/html/escape.h"

using namespace Render;

#define CHOICE(l) (std::string) \
    "<td><input name=\"license\" type=\"radio\" value=\"" + l + "\" " + \
    (t.license() == l ? found = true, "checked=\"checked\" ": "") + "/></td>"

void licenseForm(const ExtendedTrack &t){
    Html::header("License selection");
    bool found = false;
    o << "<h2>" << (t ? "License for: " + Html::escape(t.title()) : "Global license") << "</h2>"
         "<form method=\"post\"><table>"
            "<tr>"
                << CHOICE("Copyright") <<
                "<td><b>Copyright</b><div>Default license.</div></td>"
            "</tr>";
    o <<    "<tr>"
                << CHOICE("Public domain") <<
                "<td><b>Public Domain</b></td>"
            "</tr>";
    o <<    "<tr>"
                << CHOICE("CC") <<
                "<td><b>Creative Commons</b></td>"
            "</tr>";
    o <<    "<tr>"
                "<td><input name=\"license\" type=\"radio\" value=\"custom\" " << (found?"":"checked=\"checked\" ") << "/></td>"
                "<td><b>Custom: </b><input name=\"custom-license\" " << (found?"":"value=\""+escape(t.license())+"\" ") << "/></td>"
            "</tr>"
            << (t ? "<tr><td><input name=\"mkdefault\" type=\"checkbox\" /></td><td>Make it the default license.</td></tr>" : "") <<
            "<tr><td><input name=\"retro\" type=\"checkbox\" /></td><td>Apply to all tracks.</td></tr>"
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
