#ifndef TEXT_H
#define TEXT_H

#include <string>

std::string day(const std::string &date);

std::string ircNick(std::string name);

bool isNumber(const std::string &str);

std::string number(int n);
int number(const std::string &str);

#endif // TEXT_H
