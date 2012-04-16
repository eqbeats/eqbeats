#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "track.h"
#include "user.h"

class Playlist {

    public:
        Playlist(){ _id = 0; }
        Playlist(int nId);

        operator bool() const { return _id > 0; }
        int id() const { return _id; }
        int length() const { return _length; }
        User author() const { return _author; }
        std::string name() const { return _name; }
        std::string description() const { return _description; }
        std::string url() const;

        std::vector<Track> tracks();

        static Playlist create(const std::string &nName, const std::string &nDesc=std::string());
        void setMeta(const std::string &nName, const std::string &nDesc);
        void add(unsigned tid);
        void remove(unsigned pos);
        void swap(unsigned i, unsigned j);
        void drop();

        static void removeTrack(int tid);

        static std::vector<Playlist> forUser(User u);

    private:
        Playlist(int nid, User nauthor, int nlength, std::string nname, std::string ndescription)
            { _id = nid; _author = nauthor; _length = nlength; _name = nname; _description = ndescription; }
        int _id;
        User _author;
        int _length;
        std::string _name;
        std::string _description;

};

#endif //PLAYLIST_H
