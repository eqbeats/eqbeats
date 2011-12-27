#ifndef TRACK_H
#define TRACK_H

#include "db.h"

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

        std::string url() const { return Track::url(_id); }
        static std::string url(int id);

        enum Format { FLAC, Vorbis, MP3 };
        std::string url(Format f) const;

        void convert(Format f);

        static Track create(int nArtistId, const std::string &nTitle);
        void remove();

        void hit();

        static std::vector<Track> byArtist(int sArtistId, bool all=false);
        static std::vector<Track> search(const std::string &q);
        static std::vector<Track> latest(int n);
        static std::vector<Track> random(int n);
        static std::vector<Track> popular(int n);

        operator bool() const { return _id > 0; }

        /*enum Genre {
            Rock,
            Metal,
            Instrumental,
            Classical,
            Electronic,
            Techno,
            DrumNBass,
            Funk,
            Disco,
            Pop,
            Rap,
            HipHop,
            Country,
            Jazz
        };*/

        void addComment(const std::string &msg, std::string userName, int uid=0);

        class Comment {

            public:
                Comment(){}
                std::string contents() const { return _msg; }
                int authorId() const { return _authorId; }
                std::string authorName() const;
                int trackId() const { return _tid; }
                std::string trackTitle() const { return _track; }
                static std::vector<Comment> forArtist(int uid);
                static std::vector<Comment> forTrack(int tid);

            private:
                Comment(int authorId, const std::string &msg, const std::string &nAuthorName, int tid=0, const std::string &ttitle=std::string())
                    { _msg = msg; _authorId = authorId; _authorName = nAuthorName; _tid = tid; _track = ttitle; }
                std::string _msg;
                int _authorId;
                std::string _authorName;
                int _tid;
                std::string _track;

        };

    private:
        int _id;
        std::string _title;
        int _artistId;
        std::string _artist;
        bool _visible;
        Track(int nId, const std::string &nTitle, int nArtistId, const std::string &nArtist, bool nVisible)
            { _id = nId; _title = nTitle; _visible = nVisible;
              _artistId = nArtistId; _artist = nArtist; }
        static std::vector<Track> resultToVector(const DB::Result &r);
};

#endif //TRACK_H
