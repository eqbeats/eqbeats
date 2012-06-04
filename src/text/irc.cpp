#include "text.h"
#include <string.h>

std::string ircNick(std::string name){
    const char* accepted = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789|-[]{}\\`^_";
    name = name.substr(0, 20);
    std::string newname;
    for(std::string::iterator i = name.begin(); i != name.end(); i++){
        if((*i & 0b10000000) == 0 || (*i & 0b01000000) != 0){
            if(!strchr(accepted, *i)) newname += '_';
            else newname += *i;
        }
    }
    return newname;
}
