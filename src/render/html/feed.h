#ifndef HTML_FEED_H
#define HTML_FEED_H

#include <string>

namespace Render {
namespace Html {

std::string atomFeed(const std::string &url);
void feedIcon(const std::string &url);

}
}


#endif // HTML_FEED_H
