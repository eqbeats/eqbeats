#ifndef PATH_H
#define PATH_H

#include <string>

// Return 0 on error.
int route(const std::string &t, std::string path);
int route(const std::string &t, const std::string &act, std::string path);

std::string stripSlash(const std::string &path);

std::string eqbeatsDir();

#endif // PATH_H
