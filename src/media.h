#ifndef MEDIA_H
#define MEDIA_H

#include "track.h"

class Media: public Track{

    public:
        Media(const Track &t): Track(t) {}

        std::string filePath(Format f) const;

        void convertToVorbis();

        void updateTags() { updateTags(MP3); updateTags(Vorbis); }
        void updateTags(Format format);

        void unlink();

};

#endif // MEDIA_H

