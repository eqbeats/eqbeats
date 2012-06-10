#ifndef TEXT_H
#define TEXT_H

#include <string>

// string.cpp
bool drop(const std::string &sub, std::string &str);

// date.cpp
std::string day(const std::string &date);

// number.cpp
bool isNumber(const std::string &str);
std::string number(int n);
int number(const std::string &str);

#endif // TEXT_H
