#include "text.h"
#include <string.h>
#include <wchar.h>

bool drop(const std::string &s, std::string &str){
    if(s.length() > str.length())
        return false;
    if(str.substr(0, s.length()) == s){
        str = str.substr(s.length());
        return true;
    }
    return false;
}

/* Returns (size_t) -1 on failure, the length of the wide string otherwise. */
static size_t mbslen(const char *str, size_t sz) {
    size_t clen;
    mbstate_t ps;
    size_t wlen = 0;
    memset(&ps, 0, sizeof(ps));
    while (sz > 0) {
        clen = mbrlen(str, sz, &ps);
        if (clen == (size_t) -1)
            return -1;
        else if (clen == (size_t) -2)
            return -1;
        else if (clen == 0)
            return wlen;
        wlen++;
        sz -= clen;
        str += clen;
    }
    return wlen;
}

size_t wlength(const std::string &str) {
    return mbslen(str.c_str(), str.size());
}

bool validString(const std::string &str) {
    return wlength(str) != (size_t) -1;
}
