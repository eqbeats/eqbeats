#ifndef HTML_USER_H
#define HTML_USER_H

#include <string>

namespace Render {
namespace Html {

void userPage(int uid);
void userSearch(const std::string &q);
void usersPage();
void artistsPage();
void favorites(int uid);

}
}

#endif // HTML_USER_H
