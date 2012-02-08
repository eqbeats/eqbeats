#ifndef TRACK_H
#define TRACK_H

#include "db.h"
#include <string>

class Category{

    public:
        Category() { _id = 0; }
        Category(int cid);
        Category(int cid, std::string nName)
            { _id = cid; _name = nName; }
        int id() const { return _id; }
        std::string name() const { return _name; }
        std::string url() const;
        static std::vector<Category> list();
        bool operator==(const Category &c) { return c._id == _id; }
        operator bool() const { return _id > 0; }

    private:
        int _id;
        std::string _name;

};

class Track{

    public:
        Track() { _id = 0; _artistId = 0; }
        Track(int nId);
        int id() const{ return _id; }
        std::string title() const{ return _title; }
        void setTitle(const std::string &nTitle);
        int artistId() const{ return _artistId; }
        std::string artist() const{ return _artist; }

        bool visible() const{ return _visible; }
        void setVisible(bool);

        std::string getNotes() const;
        void setNotes(const std::string &nNotes);

        //time_t getTime() const;
        std::string date() const { return _date; }
        void bump();

        std::string url() const { return Track::url(_id); }
        static std::string url(int id);

        // Returns an empty string if no art exists.
        std::string artUrl() const;

        enum Format { Vorbis, MP3 };
        std::string url(Format f) const;

        void convertToVorbis();

        static Track create(int nArtistId, const std::string &nTitle);
        void remove();

        int getHits() const;
        void hit();

        std::vector<Category> getCategories() const;
        void removeCategories(const std::vector<int> &cats);
        void addCategory(int cid);

        int favoritesCount() const;

        static std::vector<Track> byArtist(int sArtistId, bool all=false);
        static std::vector<Track> byCategory(int cat);
        static std::vector<Track> forContest(int cont);
        static std::vector<Track> search(const std::string &q);
        static std::vector<Track> latest(int n);
        static std::vector<Track> random(int n);
        static std::vector<Track> popular(int n);
        static std::vector<Track> favorites(int uid);

        operator bool() const { return _id > 0; }
        bool operator==(const Track &c) { return c._id == _id; }

    private:
        int _id;
        std::string _title;
        int _artistId;
        std::string _artist;
        std::string _date;
        bool _visible;
        Track(int nId, const std::string &nTitle, int nArtistId, const std::string &nArtist, bool nVisible, const std::string &nDate)
            { _id = nId; _title = nTitle; _visible = nVisible;
              _artistId = nArtistId; _artist = nArtist; _date = nDate; }
        static std::vector<Track> resultToVector(const DB::Result &r);
};

#endif //TRACK_H
