#include "http.h"
#include "render.h"

using namespace Render;

std::string statusMsg(int stat){
    switch(stat){
        case 200: return "200 OK";
        case 303: return "303 See Other";
        case 403: return "403 Forbidden";
        case 404: return "404 Not Found";
        case 500: return "500 Server Error";
    }
    return "";
}

void Http::header(int status){
    std::string msg = statusMsg(status);
    o << (msg.empty() ? "\n\n" : "Status: " + msg + "\n\n");
}

void Http::header(const std::string &type, int status){
    o << "Content-Type: " << type << "; charset=UTF-8\n";
    header(status);
}

void Http::redirect(const std::string &location){
    o << "Location: " << location << "\n";
    header(303);
}

std::string httpFilename(const std::string &str){
    std::string buf;
    for(std::string::const_iterator i=str.begin(); i!=str.end(); i++){
        if   (*i == '"') buf += "\\\"";
        else buf += *i;
    }
    return buf;
}

void Http::download(const std::string &path, const std::string &filename, const std::string &mime, bool attachment){
    o << "X-Accel-Redirect: " << path << "\n"
         "Content-Disposition: " << (attachment?"attachment":"inline")
                                 << "; filename=\"" << httpFilename(filename) <<"\"\n"
         "Content-Type: " << mime << "\n\n";
}
