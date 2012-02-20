#ifndef ESCAPE_H
#define ESCAPE_H

#include <string>

namespace Render {
namespace Html {

std::string escape(const std::string &);
std::string escapeEmail(const std::string &);
std::string format(std::string);

}
}

#endif // ESCAPE_H
