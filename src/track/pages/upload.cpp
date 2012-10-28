#include "pages.h"
#include "../track.h"
#include <account/account.h>
#include <account/session.h>
#include <core/cgi.h>
#include <core/db.h>
#include <log/log.h>
#include <text/text.h>

#include <taglib/mpegfile.h>
#include <taglib/tag.h>
#include <unistd.h>

void Pages::trackUpload(Document *doc){

    std::string sub;
    int tid = route("track", path, sub);

    if(path != "/track/new" && !(tid && sub == "upload"))
        return;

    Track t(tid);
    if(tid && !t)
        return; // track not found

    User u = Session::user();
    if(!u) return doc->redirect(t ? t.url() : "/");

    cgicc::file_iterator qqfile = cgi.getFile("qqfile");
    bool js = cgi("js") != "no";
    std::string upfilename = (qqfile != cgi.getFiles().end() ? qqfile->getFilename() : cgi("qqfile"));

    if(js)
        doc->setTemplate("json/upload.tpl", "text/html"); // Opera

    if((t && t.artist != u) || upfilename.empty()){
        if(js) doc->dict()->SetValue("SUCCESS", "false");
        else doc->redirect(t ? t.url() : u ? u.url() : "/");
        return;
    }

    std::string::size_type ext_idx = upfilename.rfind('.');
    std::string ext = (ext_idx == std::string::npos ? ".derp" : upfilename.substr(ext_idx));

    std::string dir = eqbeatsDir() + "/tmp";
    char *tmpFile = tempnam(dir.c_str(), "eqb");
    tmpFile = (char*) realloc(tmpFile, strlen(tmpFile) + ext.size() + 1);
    strcat(tmpFile, ext.c_str());

    std::ofstream out(tmpFile, std::ios_base::binary);
    if(qqfile != cgi.getFiles().end())
        qqfile->writeToStream(out);
    else if(js)
        out << cgi.getEnvironment().getPostData();
    out.close();

    if(!tid){
        TagLib::MPEG::File f(tmpFile);
        TagLib::Tag *tag = f.tag();
        std::string title = tag ? tag->title().to8Bit(): "";
        if(title.empty())
            title = upfilename.substr(0, ext_idx);
        if(title.empty())
            title = "Untitled";
        // Create
        Account a(u.id);
        DB::Result r = DB::query(
            "INSERT INTO tracks (user_id, title, date, license) VALUES "
            "(" + number(a.id) + ", $1, 'now', $2) "
            "RETURNING id", title, a.license
        );
        if(r.empty()){
            if(js) doc->dict()->SetValue("SUCCESS", "false");
            else doc->redirect(u.url());
            return;
        }
        t.id = number(r[0][0]);
        t.title = title;
        // Other fields left blank.
    }

    log("Track uploaded: " + t.title + " (" + number(t.id) + ", " + tmpFile + ")");

    if(fork() == 0){
        freopen("/dev/null","r",stdin);
        execlp("transcode.sh", "transcode.sh", number(t.id).c_str(), tmpFile, NULL);
    }
    else
        sleep(2);

    free(tmpFile);

    if(js){
        doc->dict()->SetValue("SUCCESS", "true");
        Dict *trackDict = doc->dict()->AddSectionDictionary("TRACK");
        trackDict->SetIntValue("TID", t.id);
        trackDict->SetValue("TITLE", t.title);
    }
    else
        doc->redirect(t.url());

}
