#include "user.h"
#include "track.h"
#include "comment.h"
#include "number.h"

Comment::Type stot(const std::string &in){
    if(in == "news") return Comment::News;
    else if(in == "track") return Comment::Track;
    else if(in == "user") return Comment::User;
    else return Comment::Undefined;
}
std::string ttos(Comment::Type in){
    if(in == Comment::News) return "news";
    else if(in == Comment::Track) return "track";
    else if(in == Comment::User) return "user";
    else return "";
}

void Comment::add(const std::string &msg, std::string name, int uid, int ref, Comment::Type type){
    if(uid > 0)
        name = ::User(uid).name();
    if(!DB::query("SELECT 1 FROM comments WHERE author_name = $1 AND 'now' - date < '8s'",
        name).empty()) return;
    DB::query("INSERT INTO comments (author_id, author_name, contents, ref, type, date) "
    "VALUES (" + number(uid) + ", $1, $2, "+ number(ref) +", $3, 'now')", name, msg, ttos(type));
}

std::vector<Comment> Comment::forArtist(int uid){
    DB::Result r = DB::query(
        "SELECT comments.author_id, comments.contents, comments.author_name, comments.type, tracks.id, tracks.title "
        "FROM comments CROSS JOIN tracks "
        "WHERE comments.type = 'track' AND tracks.user_id = " + number(uid) + " AND tracks.id = comments.ref "
        "ORDER BY comments.date DESC");
    std::vector<Comment> cmts(r.size());
    for(unsigned i=0; i<r.size(); i++)
        cmts[i] = Comment(number(r[i][0]), r[i][1], r[i][2], stot(r[i][3]), number(r[i][4]), r[i][5]);
    return cmts;
}

std::vector<Comment> commentHelper(Comment::Type type, int ref){
    DB::Result r = DB::query(
        "SELECT author_id, contents, author_name FROM comments WHERE type = $1 AND ref = " + number(ref)
        + " ORDER BY date ASC", ttos(type));
    std::vector<Comment> cmts(r.size());
    for(unsigned i=0; i<r.size(); i++)
        cmts[i] = Comment(number(r[i][0]), r[i][1], r[i][2], type);
    return cmts;
}

std::vector<Comment> Comment::forUser(int uid){
    return commentHelper(User, uid);
}

std::vector<Comment> Comment::forTrack(int tid){
    return commentHelper(Track, tid);
}

std::vector<Comment> Comment::forNews(int nid){
    return commentHelper(News, nid);
}

int Comment::countForNews(int nid){
    DB::Result r = DB::query("SELECT count(*) FROM comments WHERE type = 'news' AND ref = " + number(nid));
    return r.empty() ? 0 : number(r[0][0]);
}
