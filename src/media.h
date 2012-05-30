#ifndef MEDIA_H
#define MEDIA_H

#include "track.h"

class Media: public Track{

    public:
        Media(const Track &t);

        std::string extension(Format f=Original) const;
        std::string filePath(Format f) const;
        std::string mimetype(Format f=Original) const;

        void convertToVorbis();

        void updateTags() { updateTags(MP3); updateTags(Vorbis); }
        void updateTags(Format format);

        void unlink();

    private:
        std::string ext;

};

#endif // MEDIA_H

