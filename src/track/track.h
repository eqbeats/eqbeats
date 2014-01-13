#ifndef TRACK_H
#define TRACK_H

#include <account/account.h>

class Track{

    public:

        Track() { id = 0; }
        Track(int tid);

        int id;
        std::string title;
        Account artist;
        std::string date;
        std::string timestamp;
        bool visible;

        std::string url() const;

        void fill(Dict *d) const;
        Dict* player(Dict *d, bool fallback=false) const;

        operator bool() const { return id > 0; }
        bool operator==(const Track &t) { return t.id == id; }

};

#endif //TRACK_H
