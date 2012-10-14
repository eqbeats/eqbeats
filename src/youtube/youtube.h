#ifndef YOUTUBE_H
#define YOUTUBE_H

#include <render/file.h>
#include <track/extended.h>

class Youtube{

    public:

        Youtube(int uid);

        operator bool() const;

        void unlink();
        bool link(std::string & code);
        bool upload(ExtendedTrack & t);

    private:

        int _uid;

};

#endif // YOUTUBE_H
