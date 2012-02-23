#ifndef MEDIA_H
#define MEDIA_H

#include "track.h"

class Media: public Track{

    public:
        Media(const Track &t): Track(t) {}

        void convertToVorbis();

        void updateTags() { updateTags(Track::MP3); updateTags(Track::Vorbis); }
        void updateTags(Track::Format format);

};

#endif // MEDIA_H

