#include "pages.h"
#include "../art.h"
#include "../track.h"
#include <account/session.h>
#include <core/cgi.h>
#include <text/text.h>

void Pages::art(Document *doc){

    std::string sub;
    int tid = route("track", path, sub);
    if(!tid) return;
    if(sub.substr(0,3) != "art") return;
    Track t(tid);

    // Upload
    if(sub == "art/upload"){
        cgicc::file_iterator file = cgi.getFile("file");
        if(t.artist.self() && file != cgi.getFiles().end()){
            std::string filename = eqbeatsDir() + "/art/" + number(t.id);
            std::ofstream out(filename.c_str(), std::ios_base::binary);
            file->writeToStream(out);
            out.close();
            Art art(t.id);
            art.makeThumbs();
        }
        return doc->redirect(t.url());
    }

    // Download
    Art art(tid);
    if(art){
        std::string base = t.artist.name + " - " + t.title;
             if(sub == "art")        doc->download(art.full().setBaseName(base));
        else if(sub == "art/medium") doc->download(art.medium().setBaseName(base + ".medium"));
        else if(sub == "art/thumb")  doc->download(art.thumbnail().setBaseName(base + ".thumb"));
    }

}
