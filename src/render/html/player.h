#ifndef HTML_PLAYER_H
#define HTML_PLAYER_H

#include <string>

class Track;

namespace Render {
namespace Html {

void embedTrack(int tid);
std::string embedTrackCode(const Track &t, int w=0);
void player(const Track &t);

}
}

#endif // HTML_PLAYER_H
