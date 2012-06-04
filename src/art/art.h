#ifndef ART_H
#define ART_H

#include <string>

class Art{

    public:
        // Sets _tid = 0 on error
        Art(int tid);

        enum Size { Full, Medium, Thumbnail };

        std::string filepath(Size sz=Full) const;
        std::string url(Size sz=Full) const;

        void makeThumbs();

        enum Format { Unknown, PNG, JPEG, GIF, TIFF };
        Format getFormat();

        void remove();

        operator bool(){ return _tid>0; }

    private:
        int _tid;

};

#endif // ART_H
