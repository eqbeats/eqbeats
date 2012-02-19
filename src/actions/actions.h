#ifndef ACTIONS_H
#define ACTIONS_H

#include "../comment.h"
#include <cgicc/Cgicc.h>

//! Page generation with effects
namespace Action{

extern cgicc::Cgicc *cgi;

// login.cpp
std::string login();
std::string logout();

// registration.cpp
std::string registration();
std::string account();

// track.cpp
std::string deleteTrack(int tid);
std::string renameTrack(int tid);
std::string updateNotes(int tid);
std::string publishTrack(int tid);
std::string updateCategories(int tid);

// upload.cpp
std::string uploadTrack(int tid);
std::string newTrack();

// comment.cpp
std::string postComment(Comment::Type type, int ref);

// art.cpp
std::string uploadArt(int tid);

// favorite.cpp
std::string follow(int uid, bool add);
std::string favorite(int tid, bool add);
std::string vote(int cid);

// contest.cpp
std::string contestSubmission(int cid);

}

#endif // ACTIONS_H
