#ifndef FEATURE_H
#define FEATURE_H

#include <playlist/playlist.h>
#include <text/text.h>

class Feature{
    public:
        enum Type { None, Track, Playlist };

        Feature(unsigned uid);

        Type type;
        unsigned tid;
        ::Playlist playlist;
        TrackList tracklist;

        void fill(Dict *d);

        operator bool() const { return type != None; }
};

#endif
