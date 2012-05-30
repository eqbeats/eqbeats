#ifndef TRACK_H
#define TRACK_H

#include <vector>
#include <string>

class User;

class Track{

    public:
        Track() { _id = 0; _artistId = 0; }
        Track(int nId);
        Track(int nId, std::string nTitle){ _id = nId; _title = nTitle; }

        int id() const{ return _id; }
        std::string title() const{ return _title; }
        User artist() const;
        bool visible() const{ return _visible; }
        std::string date() const { return _date; }

        void setTitle(const std::string &nTitle);
        void setVisible(bool);
        void bump();

        std::string url() const { return Track::url(_id); }
        static std::string url(int id);

        enum Format { Vorbis, MP3, Original };
        std::string url(Format f) const;
        std::string filePath(Format f) const;

        static Track create(int nArtistId, const std::string &nTitle);
        void remove();

        int getHits() const;
        int hit();

        int favoritesCount() const;

        static std::vector<Track> search(const std::string &q);
        static std::vector<Track> exactSearch(const std::string &qartist, const std::string &qtitle);
        static std::vector<Track> byTag(const std::string &tag);
        static std::vector<Track> byPlaylist(const int &id);

        static std::vector<Track> select(const char *tables, const std::string cond, const char *order="tracks.date DESC", bool all=false, int limit=0, int offset=0);

        // Track lists
        static std::vector<Track> latest(int n, int offset=0);
        static std::vector<Track> random(int n);
        static std::vector<Track> featured(int n);
        static std::vector<Track> favorites(int uid);

        operator bool() const { return _id > 0; }
        bool operator==(const Track &c) { return c._id == _id; }

    protected:
        int _id;
        std::string _title;
        int _artistId;
        std::string _artist;
        std::string _date;
        bool _visible;

    private:
        Track(int nId, const std::string &nTitle, int nArtistId, const std::string &nArtist, bool nVisible, const std::string &nDate)
            { _id = nId; _title = nTitle; _visible = nVisible;
              _artistId = nArtistId; _artist = nArtist; _date = nDate; }
        static std::vector<Track> resultToVector(const std::vector<std::vector<std::string> > &r);

};

class ExtendedTrack: public Track {

    public:
        ExtendedTrack(int tid);

        std::string notes() const { return _notes; }
        bool airable() const { return _airable; }
        std::vector<std::string> tags() const { return _tags; }
        std::string license() const { return _license; }

        void setAirable(bool);
        void setNotes(const std::string &nNotes);
        void setLicense(const std::string &nLicense);
        void setTags(const std::string &nTags);

    private:
        std::string _notes;
        bool _airable;
        std::vector<std::string> _tags;
        std::string _license;

};

#endif //TRACK_H
