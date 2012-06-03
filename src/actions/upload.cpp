#include "actions.h"
#include "../render/render.h"
#include "../render/html/page.h"
#include "../render/http.h"
#include "../render/json.h"
#include "../session.h"
#include "../number.h"
#include "../path.h"
#include "../log.h"
#include "../track.h"
#include "../user.h"
#include "../cgi.h"
#include "../media.h"
#include <string.h>
#include <fstream>
#include <taglib/mpegfile.h>
#include <taglib/tag.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace Render;
using namespace Json;

std::string getTitle(const char *filename){
    TagLib::MPEG::File f(filename);
    TagLib::Tag *t = f.tag();
    return t? t->title().to8Bit(): "";
}

void Action::uploadTrack(int id){
    User u = Session::user();
    if(!u) return Http::redirect(Track::url(id));

    cgicc::file_iterator qqfile = cgi.getFile("qqfile");
    bool js = cgi("js") != "no";
    std::string upfilename = (qqfile != cgi.getFiles().end() ? qqfile->getFilename() : cgi("qqfile"));

    if(js) Http::header("text/html"); // Opera

    Track t(id);
    if((t && t.artist() != u) || upfilename.empty()){
        if(js) o << "{" << field("success","false",true) << "}";
        else Http::redirect(t ? t.url() : u ? u.url() : "/");
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

    if(id == -1){
        std::string title = getTitle(tmpFile);
        if(title.empty())
            title = upfilename.substr(0, ext_idx);
        t = Track::create(u.id(), title.empty()? "Untitled":title);
    }

    log("Track uploaded: " + t.title() + " (" + number(t.id()) + ")");

    Media m(t);
    if(fork() == 0){
        freopen("/dev/null","r",stdin);
        execlp("transcode.sh", "transcode.sh", number(m.id()).c_str(), tmpFile, NULL);
        free(tmpFile);
    }
    else
        sleep(2);

    if(js)
        o << "{"+field("success","true")+field("track",number(t.id()))+field("title",jstring(t.title()),true)+"}";
    else Http::redirect(t.url());
}

void Action::newTrack(){
    uploadTrack(-1);
}
