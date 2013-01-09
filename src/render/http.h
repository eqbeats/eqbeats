#ifndef HTTP_H
#define HTTP_H

#include "file.h"

namespace Http{

std::string header(int status=200);
std::string header(const std::string &type, int status=200);

std::string redirect(const std::string &location);
std::string download(const File &f, bool attachment);

}

#endif // HTTP_H
