#ifndef HTML_CONTEST_H
#define HTML_CONTEST_H

#include <string>
#include <vector>

class Contest;
class Track;

namespace Render{
namespace Html{

void contest(int cid);
void submissions(const Contest &c, const std::vector<Track> &tracks);

}
}

#endif //HTML_CONTEST_H
