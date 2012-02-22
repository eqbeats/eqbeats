#ifndef HTML_TRACK_H
#define HTML_TRACK_H

#include <vector>
#include <string>

class Track;

namespace Render{
namespace Html{

void trackPage(int tid);

enum TrackList {
    Standard,
    //! Artist not displayed.
    Compact,
};

void trackList(const std::vector<Track> &tracks, TrackList l=Standard);
void tracksPage(const std::string &title, const std::vector<Track> &tracks);
void latestTracks(int n);
void trackSearch(const std::string &q);
void category(int cid);

}
}

#endif //HTML_TRACK_H
