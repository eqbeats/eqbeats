#ifndef MEDIA_AUDIO_H
#define MEDIA_AUDIO_H

#include "track.h"
#include <media/file.h>

class Audio{

    public:

        Audio(const Track *t) : track(t) {}

        File mp3() const;
        File vorbis() const;
        File original() const;

        void updateTags(){ updateTags(MP3); updateTags(Vorbis); }
        void unlink();

        void fill(Dict*);

    private:
        enum Format { MP3, Vorbis, Original };
        std::string filepath(Format f) const;
        void updateTags(Format format);

        const Track *track;

};

#endif // MEDIA_AUDIO_H
