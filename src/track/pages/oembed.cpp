#include "pages.h"
#include "../track.h"
#include <core/cgi.h>
#include <text/text.h>

void Pages::oembed(Document *doc){
    if(path != "/oembed") return;

    std::string url = cgi("url");
    drop("http://", url);
    drop("www.", url);
    if(drop("eqbeats.org", url)){
        std::string s;
        int tid = route("track", url, s);
        if(s == ""){
            Track t(tid);
            if(t){
                if(cgi("format") == "xml")
                    doc->setTemplate("oembed/xml.tpl", "text/xml");
                else
                    doc->setJson("oembed/json.tpl");
                t.fill(doc->dict());
                int width = number(cgi("maxwidth"));
                if(width <= 0) width = 150;
                doc->dict()->SetIntValue("WIDTH", width);
                Dict *embed = doc->dict()->AddIncludeDictionary("EMBED_CODE");
                embed->SetFilename("html/embed-code.tpl");
                embed->SetIntValue("WIDTH", width);
                t.Track::fill(embed);
            }
        }
    }
}
