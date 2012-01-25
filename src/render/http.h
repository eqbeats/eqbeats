#ifndef HTTP_H
#define HTTP_H

#include <string>

namespace Http{

std::string header(int status=200);
std::string header(const std::string &type, int status=200);

std::string notFound();
std::string redirect(const std::string &location);

}

#endif // HTTP_H
