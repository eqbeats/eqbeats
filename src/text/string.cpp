#include "text.h"
#include <ctype.h>

bool drop(const std::string &s, std::string &str){
    if(s.length() > str.length())
        return false;
    if(str.substr(0, s.length()) == s){
        str = str.substr(s.length());
        return true;
    }
    return false;
}

// lowercase, replace spaces by -, and drop all non-[a-z]-characters
bool urlize(const std::string &s, std::string &str) {
    str.clear();
    str.reserve(s.size());
    std::string::const_iterator it;
    for( it=s.begin() ; it < s.end(); it++ ) {
        char x = (*it);
        if(isspace(x))
            str.append("-");
        if(isalnum(x))
            str.append(sizeof(char), tolower(x));
    }

    return str.size() > 0;
}
