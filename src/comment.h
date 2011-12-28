#ifndef COMMENT_H
#define COMMENT_H

class Comment {

    public:

        Comment(){ authorId = 0; type = Undefined; trackId = 0; }
        enum Type { Undefined, Track, News };

        Type type;
        std::string contents;
        int authorId;
        std::string _authorName;
        std::string authorName() const{ return _authorName.empty()? "Anonymous" : _authorName; }

        int trackId;
        std::string trackTitle;

        static void add(const std::string &msg, std::string name, int uid, int ref, Type type);

        static std::vector<Comment> forArtist(int uid);
        static std::vector<Comment> forTrack(int tid);
        static std::vector<Comment> forNews(int nid);

        Comment(int nAuthorId, const std::string &nContents, const std::string &nAuthorName, Type nType, int nTrackId = 0,
                const std::string &nTrackTitle = "")
            { authorId = nAuthorId; contents = nContents; _authorName = nAuthorName; type = nType; trackId = nTrackId;
              trackTitle = nTrackTitle; }

};

#endif // COMMENT_H
