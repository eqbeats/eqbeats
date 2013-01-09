#ifndef MEDIA_AUDIO_H
#define MEDIA_AUDIO_H

#include "track.h"
#include <render/file.h>

class Audio{

    public:

        Audio(const Track *t) : track(t) {}

        File mp3() const;
        File vorbis() const;
        File aac() const;
        File opus() const;
        File original() const;

        void updateTags(){ updateTags(MP3); updateTags(Vorbis);
            updateTags(AAC); updateTags(Opus); }
        void unlink();

        void fill(Dict*);

    private:
        enum Format { MP3, Vorbis, Original, AAC, Opus };
        std::string filepath(Format f) const;
        void updateTags(Format format);

        const Track *track;

};

#endif // MEDIA_AUDIO_H
