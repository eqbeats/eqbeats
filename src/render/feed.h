#ifndef FEED_H
#define FEED_H

#include <string>

namespace Feed{

std::string latest(int n);
std::string user(int uid);
std::string category(int cid);

}

#endif // FEED_H
