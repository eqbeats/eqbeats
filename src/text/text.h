#ifndef TEXT_H
#define TEXT_H

#include <string>

// string.cpp
bool drop(const std::string &sub, std::string &str);
bool urlize(const std::string &s, std::string &str);

// date.cpp
std::string formatTime(const std::string &date, const char *fmt);
std::string fuzzyTime(const std::string &date, bool capitalize=true);

// email.cpp
bool validEmail(const std::string &email);

// number.cpp
bool isNumber(const std::string &str);
std::string number(int n);
int number(const std::string &str);

#endif // TEXT_H
