#ifndef ART_H
#define ART_H

#include <string>

class Art{

    public:
        // Sets _tid = 0 on error
        Art(int tid);

        std::string filepath() const;
        std::string url() const;

        enum Format { Unknown, PNG, JPEG };
        Format getFormat();

        operator bool(){ return _tid; }

    private:
        int _tid;

};

#endif // ART_H
