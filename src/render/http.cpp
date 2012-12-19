#include "http.h"
#include <string.h>

using namespace std;

const char* statusMsg(int stat){
    switch(stat){
        case 200: return "200 OK";
        case 303: return "303 See Other";
        case 403: return "403 Forbidden";
        case 404: return "404 Not Found";
        case 500: return "500 Server Error";
    }
    return "";
}

string Http::header(int status){
    string msg = statusMsg(status);
    return (msg.empty() ? "\n\n" : "Status: " + msg + "\n\n");
}

string Http::header(const string &type, int status){
    return "Content-Type: " + type + "; charset=UTF-8\n" + header(status);
}

string Http::redirect(const string &location){
    return "Location: " + location + "\n" + header(303);
}

string httpFilename(const string &str){
    string buf;
    for(string::const_iterator i=str.begin(); i!=str.end(); i++){
        if(strchr("/\\?%*:|\"<>", *i) != 0);
        else buf += *i;
    }
    return buf;
}

string Http::download(const File &f){
    return (string)
        "X-Accel-Redirect: /downloads/" + f.path() + "\n"
        "Content-Disposition: attachment; filename=\"" + httpFilename(f.filename()) +"\"\n"
        "Content-Type: " + f.mimetype() + "\n\n";
}
