#ifndef JSON_H
#define JSON_H

#include "../track.h"
#include "../user.h"

namespace Json{

std::string field(const std::string &name, const std::string &val, bool last=false);
std::string jstring(const std::string &);

std::string track(const Track &t);
std::string tracks(const std::vector<Track> &);

std::string artist(int uid);
std::string users(const std::vector<User> &);

std::string category(int cid);

}

#endif // JSON_H
