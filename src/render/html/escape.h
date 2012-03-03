#ifndef HTML_ESCAPE_H
#define HTML_ESCAPE_H

#include <string>

namespace Render {
namespace Html {

std::string escape(const std::string &);
std::string escapeEmail(const std::string &);

std::string format(std::string);
std::string strip(const std::string&);

}
}

#endif // HTML_ESCAPE_H
