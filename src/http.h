#ifndef HTTP_H
#define HTTP_H

#include <string>

namespace Http{

std::string header(int status=200);
std::string header(const std::string &type, int status=200);

std::string redirect(const std::string &location);
std::string download(const std::string &path, const std::string &filename, const std::string &mime="application/octet-stream", bool attachment=false);

}

#endif // HTTP_H
