#ifndef JSON_H
#define JSON_H

#include <vector>
#include <string>

class Track;
class User;

namespace Render {
namespace Json {

std::string field(const std::string &name, const std::string &val, bool last=false);
std::string jstring(const std::string &);

void track(int tid);
void tracks(const std::vector<Track> &);

void artist(int uid);
void users(const std::vector<User> &);

void category(int cid);

void nowPlaying(const std::string &file);

}
}

#endif // JSON_H
