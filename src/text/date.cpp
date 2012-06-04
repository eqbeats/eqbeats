#include "text.h"

std::string day(const std::string &date){
    struct tm tm;
    strptime(date.c_str(), "%F", &tm);
    char day_str[256];
    strftime(day_str, 256, "%B %-d, %Y", &tm);
    return day_str;
}
