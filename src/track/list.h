#ifndef TRACK_LIST_H
#define TRACK_LIST_H

#include "track.h"

#include <vector>

class TrackList : public std::vector<Track> {

    public:

        TrackList(const std::string &query_fmt, bool all=false);

        Dict* fill(Dict*, std::string section);

};

namespace Tracks{

    TrackList latest(int n);
    TrackList featured(int n);
    TrackList random(int n);

}

#endif // TRACK_LIST_H
