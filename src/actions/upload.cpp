#include "actions.h"
#include "../html/html.h"
#include "../session.h"
#include "../utils.h"
#include <string.h>
#include <fstream>
#include <taglib/mpegfile.h>
#include <taglib/tag.h>
#include <sys/stat.h>

std::string getTitle(const char *filename){
    TagLib::MPEG::File f(filename);
    TagLib::Tag *t = f.tag();
    return t? t->title().to8Bit(): "";
}

std::string Action::uploadTrack(int id, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(id);

    static const std::string header = "Content-Type: text/html\n\n";
    cgicc::file_iterator file = cgi.getFile("file");
    cgicc::file_iterator qqfile = cgi.getFile("qqfile");
    bool isXhr = !cgi("qqfile").empty() || qqfile != cgi.getFiles().end();

    std::string formatError = isXhr? header+"{ success: false, error: 'Only MP3 files are accepted.' }" : Html::errorPage("Only MP3 files are accepted.");
    std::string genericError = isXhr? header+"{ success: false }" : Html::redirect(t?t.url():u?u.url():"/");

    if(!u) return genericError;
    if(t && t.artistId() != u.id())
        return genericError;

    std::string dir = eqbeatsDir() + "/tmp";
    char *tmpFile = tempnam(dir.c_str(), "eqb");
    std::ofstream out(tmpFile, std::ios_base::binary);
    std::string upfilename;

    if(qqfile != cgi.getFiles().end()) {
        qqfile->writeToStream(out);
        upfilename = qqfile->getName();
    } else if(isXhr) {
        out << cgi.getEnvironment().getPostData();
        upfilename = cgi("qqfile");
    } else if(file != cgi.getFiles().end()) {
        file->writeToStream(out);
        upfilename = file->getName();
    }
    out.close();

    // check filesize
    struct stat info;
    if(stat(tmpFile, &info) == 0){
        if(info.st_size < 500)
            return formatError;
    }
    else return genericError;

    if(id == -1){
        std::string title = getTitle(tmpFile);
        if(title.empty())
            title = upfilename.substr(0, upfilename.rfind('.'));
        t = Track::create(u.id(), title.empty()? "Untitled":title);
    }

    std::string filename = eqbeatsDir() + "/tracks/"+number(t.id())+".mp3";
    rename(tmpFile, filename.c_str());
    free(tmpFile);

    t.convertToVorbis();

    return isXhr? header+"{ success: true, track: " + number(t.id()) + ", title: " + Html::jsonString(t.title()) + " }" : Html::redirect(t.url());
}

std::string Action::newTrack(cgicc::Cgicc &cgi){
    return uploadTrack(-1, cgi);
}
