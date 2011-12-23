#include "actions.h"
#include "../track.h"
#include "../html/html.h"
#include <string.h>
#include <string>
#include <iostream>
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
    Track t(id);
    bool isXhr = !cgi("qqfile").empty();
    if(!t) return isXhr? "\n\n{ success: false }" : Html::redirect("/");
    const std::string data = isXhr? xhrUpload(cgi) : formUpload(cgi);
    std::string u = t.pageUrl();
    if(memcmp(data.c_str(), "fLaC", 4))
        return isXhr? "\n\n{ success: false, error: 'Only FLAC files are accepted.' }" : Html::redirect(u + "?error=flac");
    char filename[100];
    sprintf(filename, "/srv/eqbeats/tracks/%d.flac", id);
    std::ofstream f(filename, std::ios_base::binary);
    f << data;
    f.close();
    t.createMp3();
    return isXhr? "\n\n{ success: true }" : Html::redirect(u);
}
