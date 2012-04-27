#ifndef HTML_USER_H
#define HTML_USER_H

#include <string>

namespace Render {
namespace Html {

void userPage(int uid);
void userSearch(const std::string &q);
void artistsPage(unsigned n);
void favorites(int uid);

}
}

#endif // HTML_USER_H
