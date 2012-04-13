#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "track.h"

class Playlist {

    public:
        Playlist(){}
        Playlist(int nId);

        operator bool() const { return _id > 0; }
        int id(){ return _id; }
        int length(){ return _length; }
        std::string name(){ return _name; }
        std::string description(){ return _description; }

    private:
        std::vector<int> tracks;
        int _id;
        int _length;
        std::string _name;
        std::string _description;
}
#endif //PLAYLIST_H
