#include "actions.h"
#include "../html/html.h"
#include "../session.h"
#include "../utils.h"
#include "../flac.h"
#include <string.h>
#include <fstream>

const std::string xhrUpload(cgicc::Cgicc &cgi){
    return cgi.getEnvironment().getPostData();
}

const std::string formUpload(cgicc::Cgicc &cgi){
    cgicc::file_iterator file = cgi.getFile("file");
    if (file != cgi.getFiles().end())
        return file->getData();
    return std::string();
}

std::string Action::uploadTrack(int id, cgicc::Cgicc &cgi){
    User u = Session::user();
    Track t(id);
    static const std::string json = "Content-Type: application/json\n\n";
    bool isXhr = !cgi("qqfile").empty();
    std::string flacError = isXhr? json+"{ success: false, error: 'Only FLAC files are accepted.' }" : Html::errorPage("Only FLAC files are accepted.");
    std::string genericError = isXhr? json+"{ success: false }" : Html::redirect("/");
    if(!u) return genericError;
    const std::string data = isXhr? xhrUpload(cgi) : formUpload(cgi);
    if(memcmp(data.c_str(), "fLaC", 4)) return flacError;
    FlacDecoder flac(data.c_str(), data.length());
    if(!flac.ok()) return flacError;
        
    if(id == -1){
        std::string title = flac.title().empty()? "Untitled" : flac.title();
        t = Track::create(u.id(), title);
    }
    else if(t.artistId() != u.id())
        return genericError;

    std::string filename = EQBEATS_DIR"/tracks/"+number(t.id())+".flac";
    std::ofstream f(filename.c_str(), std::ios_base::binary);
    f << data;
    f.close();
    t.convert(Track::Vorbis);
    t.convert(Track::MP3);
    return isXhr? json+"{ success: true, track: " + number(t.id()) + ", title: \"" + Html::escape(t.title()) + "\" }" : Html::redirect(t.url());
}

std::string Action::newTrack(cgicc::Cgicc &cgi){
    return uploadTrack(-1, cgi);
}
