#include "user.h"
#include "track.h"
#include "comment.h"
#include "db.h"
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
