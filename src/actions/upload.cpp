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
#include "../media.h"
#include <string.h>
#include <fstream>
#include <taglib/mpegfile.h>
#include <taglib/tag.h>
#include <sys/stat.h>

using namespace Render;
using namespace Json;

std::string getTitle(const char *filename){
    TagLib::MPEG::File f(filename);
    TagLib::Tag *t = f.tag();
    return t? t->title().to8Bit(): "";
}

void Action::uploadTrack(int id){
    User u = Session::user();
    Track t(id);

    cgicc::file_iterator qqfile = cgi.getFile("qqfile");
    bool js = cgi("js") != "no";

    if(js) Http::header("text/html"); // Opera

    if(t && t.artist() != u){
        if(js) o << "{" << field("success","false",true) << "}";
        else Http::redirect(u ? u.url() : "/");
        return;
    }

    std::string dir = eqbeatsDir() + "/tmp";
    char *tmpFile = tempnam(dir.c_str(), "eqb");
    std::ofstream out(tmpFile, std::ios_base::binary);
    std::string upfilename;

    if(qqfile != cgi.getFiles().end())
        qqfile->writeToStream(out);
    else if(js) {
        out << cgi.getEnvironment().getPostData();
        upfilename = cgi("qqfile");
    }
    out.close();

    if(id == -1){
        std::string title = getTitle(tmpFile);
        if(title.empty())
            title = upfilename.substr(0, upfilename.rfind('.'));
        t = Track::create(u.id(), title.empty()? "Untitled":title);
    }

    std::string filename = eqbeatsDir() + "/tracks/"+number(t.id())+".mp3";
    rename(tmpFile, filename.c_str());
    free(tmpFile);

    log("Track uploaded: " + t.title() + " (" + number(t.id()) + ")");

    Media m(t);
    m.updateTags(Track::MP3);
    m.convertToVorbis();
    if(js)
        o << "{"+field("success","true")+field("track",number(t.id()))+field("title",jstring(t.title()),true)+"}";
    else Http::redirect(t.url());
}

void Action::newTrack(){
    uploadTrack(-1);
}
