#ifndef TRACK_LIST_H
#define TRACK_LIST_H

#include "track.h"

#include <vector>

class TrackList : public std::vector<Track> {

    public:

        TrackList(const char *query_fmt);

        Dict* fill(Dict*, std::string section);

};

#endif // TRACK_LIST_H
