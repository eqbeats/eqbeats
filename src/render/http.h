#ifndef HTTP_H
#define HTTP_H

#include <string>

namespace Http{

void header(int status=200);
void header(const std::string &type, int status=200);

void redirect(const std::string &location);
void download(const std::string &path, const std::string &filename, const std::string &mime="application/octet-stream", bool attachment=false);

}

#endif // HTTP_H
