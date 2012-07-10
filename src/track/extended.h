#ifndef TRACK_EXTENDED_H
#define TRACK_EXTENDED_H

#include "track.h"

class ExtendedTrack: public Track {

    public:

        ExtendedTrack(int tid);

        std::string notes;
        bool airable;
        std::vector<std::string> tags;
        std::string license;

        int hit();
        int getHits();

        void fill(Dict*) const;

};

#endif // TRACK_EXTENDED_H
