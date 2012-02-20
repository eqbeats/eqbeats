#include "actions.h"
#include "../render/html.h"
#include "../render/http.h"
#include "../render/json.h"
#include "../session.h"
#include "../number.h"
#include "../path.h"
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

    cgicc::file_iterator file = cgi->getFile("file");
    cgicc::file_iterator qqfile = cgi->getFile("qqfile");
    bool isXhr = !(*cgi)("qqfile").empty();

    isXhr ? Http::header("application/json") : Http::redirect(t?t.url():u?u.url():"/");

    if(!u || !t || t.artistId() != u.id()){
        if(isXhr)
            o << "{" << field("success","false",true) << "}";
        return;
    }

    std::string dir = eqbeatsDir() + "/tmp";
    char *tmpFile = tempnam(dir.c_str(), "eqb");
    std::ofstream out(tmpFile, std::ios_base::binary);
    std::string upfilename;

    if(qqfile != cgi->getFiles().end()) {
        qqfile->writeToStream(out);
        upfilename = qqfile->getName();
    } else if(isXhr) {
        out << cgi->getEnvironment().getPostData();
        upfilename = (*cgi)("qqfile");
    } else if(file != cgi->getFiles().end()) {
        file->writeToStream(out);
        upfilename = file->getName();
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

    t.updateTags(Track::MP3);
    t.convertToVorbis();

    if(isXhr)
        o << "{"+field("success","true")+field("track",number(t.id()))+field("title",jstring(t.title()),true)+"}";
}

void Action::newTrack(){
    uploadTrack(-1);
}
