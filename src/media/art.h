#ifndef MEDIA_ART_H
#define MEDIA_ART_H

#include "file.h"

class Art{

    public:

        Art(int tid);

        File thumbnail() const;
        File medium() const;
        File full() const;

        void makeThumbs();
        void remove();

        operator bool(){ return _tid>0; }

    private:
        enum Size { Full, Medium, Thumbnail };
        std::string filepath(Size sz=Full) const;

        int _tid;

};

#endif // MEDIA_ART_H
