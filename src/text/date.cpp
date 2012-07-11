#include "text.h"
#include <string.h>

std::string formatTime(const std::string &date, const char *fmt){
    struct tm tm;
    strptime(date.c_str(), "%F %T", &tm);
    char day_str[256];
    strftime(day_str, 256, fmt, &tm);
    return day_str;
}

std::string fuzzyTime(const std::string &date, bool capitalize){
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(date.c_str(), "%F %T", &tm);
    time_t now = time(0);
    int secs = difftime(mktime(gmtime(&now)), mktime(&tm));
    if(secs < 0){
        return (std::string)(capitalize?"I":"i") + "n the future";
    } else if(secs < 5){
        return (std::string)(capitalize?"A":"a") + " few seconds ago";
    } else if(secs < 60){
        return (std::string)(capitalize?"L":"l") + "ess than a minute ago";
    } else if(secs < 120){
        return (std::string)(capitalize?"A":"a") + " minute ago";
    } else if(secs < 60*60*3){
        int mins = (secs / 60) % 60;
        int hours = secs / 3600;
        return (hours!=0?(hours==1?(std::string)(capitalize?"A":"a") + "n hour, ":number(hours)+" hours, "):"") + number(mins) + " minute" + (mins<=1?"":"s") + " ago";
    } else if(secs < 60*60*48){
        return number(secs/(60*60)) + " hours ago";
    } else if (secs < 60*60*24*60){
        return number(secs/(60*60*24)) + " days ago";
    } else if (secs < 60*60*24*30*24){
        return number(secs/(60*60*24*30)) + " months ago";
    } else {
        return number(secs/(60*60*24*30*12)) + " years ago";
    }
}
