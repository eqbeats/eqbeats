#include "user.h"
#include "track.h"
#include "utils.h"

void Track::addComment(const std::string &msg, std::string name, int uid){
    if(uid > 0)
        name = User(uid).name();
    DB::query("INSERT INTO comments (track_id, author_id, author_name, contents, date) VALUES (" + number(_id) + ", " + number(uid) + ", $1, $2, 'now')", name, msg);
}

std::vector<Track::Comment> Track::Comment::forArtist(int uid){
    DB::Result r = DB::query(
        "SELECT comments.author_id, comments.contents, comments.author_name, tracks.id, tracks.title FROM comments CROSS JOIN tracks WHERE tracks.user_id = " + number(uid) + " AND tracks.id = comments.track_id ORDER BY comments.date DESC");
    std::vector<Comment> cmts(r.size());
    for(unsigned i=0; i<r.size(); i++)
        cmts[i] = Comment(number(r[i][0]), r[i][1], r[i][2], number(r[i][3]), r[i][4]);
    return cmts;
}

std::vector<Track::Comment> Track::Comment::forTrack(int tid){
    DB::Result r = DB::query(
        "SELECT author_id, contents, author_name FROM comments WHERE track_id = " + number(tid)
        + " ORDER BY date DESC");
    std::vector<Comment> cmts(r.size());
    for(unsigned i=0; i<r.size(); i++)
        cmts[i] = Comment(number(r[i][0]), r[i][1], r[i][2]);
    return cmts;
}

std::string Track::Comment::authorName() const{
    return
        _authorName.empty()? "Anonymous" : _authorName;
}
