#ifndef HTML_TRACK_H
#define HTML_TRACK_H

#include <vector>
#include <string>

class Track;

namespace Render{
namespace Html{

void trackPage(int tid);

void trackListEntry(const Track t, std::string list, std::string extraButtons="");
void trackList(const std::vector<Track> &tracks, const std::string &list="default");
void tracksPage(const std::string &title, const std::vector<Track> &tracks);
void latestTracks(unsigned n);
void trackSearch(const std::string &q);

}
}

#endif //HTML_TRACK_H
