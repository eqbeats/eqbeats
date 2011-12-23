#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <vector>

class Track{

    public:
        Track() { _id = -1; _artistId = -1; }
        Track(int nId);
        int id() const{ return _id; }
        std::string title() const{ return _title; }
        void setTitle(const std::string &nTitle);
        int artistId() const{ return _artistId; }
        std::string artist() const{ return _artist; }

        bool visible() const{ return _visible; }
        void setVisible(bool);

        std::string pageUrl() const { return Track::pageUrl(_id); }
        static std::string pageUrl(int id);

        // Returns an empty string if the file doesn't exist.
        std::string flacUrl() const;
        //std::string vorbisUrl() const;
        std::string mp3Url() const;

        void createMp3();

        static std::vector<Track> byArtist(int sArtistId, bool all=false);

        operator bool() const { return _id > 0; }

    private:
        int _id;
        std::string _title;
        int _artistId;
        std::string _artist;
        bool _visible;
        Track(int nId, const std::string &nTitle, int nArtistId, const std::string &nArtist, bool nVisible)
            { _id = nId; _title = nTitle; _visible = nVisible;
              _artistId = nArtistId; _artist = nArtist; }
};

#endif //TRACK_H
