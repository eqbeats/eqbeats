#ifndef TRACK_LIST_H
#define TRACK_LIST_H

#include "track.h"
#include <core/db.h>

#include <vector>

class TrackList : public std::vector<Track> {

    public:

        TrackList(){}
        TrackList(const std::string &query_fmt, bool all=false);

        static TrackList tag(const std::string&);

        Dict* fill(Dict*, std::string section);

    private:
        void extract(const DB::Result &r);

};

namespace Tracks{

    TrackList latest(int n, int offset=0);
    TrackList featured(int n);
    TrackList random(int n);

}

#endif // TRACK_LIST_H
