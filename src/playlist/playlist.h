#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <account/user.h>
#include <render/template.h>
#include <track/list.h>

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

        TrackList tracks();

        void add(unsigned tid);
        void remove(unsigned pos);
        void swap(unsigned i, unsigned j);

        static void removeTrack(int tid);

        static std::vector<Playlist> forUser(const User &u);

        void fill(Dict *d) const;

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
