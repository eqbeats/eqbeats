#ifndef UTILS_H
#define UTILS_H

bool isNumber(const std::string &str);

std::string number(int n);
int number(const std::string &str);

extern std::string serverName;
std::string eqbeatsDir();
std::string eqbeatsUrl();

#endif // UTILS_H
