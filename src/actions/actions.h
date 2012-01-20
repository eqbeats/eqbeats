#ifndef ACTIONS_H
#define ACTIONS_H

#include <cgicc/Cgicc.h>
#include "../comment.h"

//! Page generation with effects
namespace Action{

// login.cpp
std::string login(cgicc::Cgicc &cgi);
std::string logout(cgicc::Cgicc &cgi);

// registration.cpp
std::string registration(cgicc::Cgicc &cgi);
std::string account(cgicc::Cgicc &cgi);

// track.cpp
std::string deleteTrack(int tid, cgicc::Cgicc &cgi);
std::string renameTrack(int tid, cgicc::Cgicc &cgi);
std::string updateNotes(int tid, cgicc::Cgicc &cgi);
std::string publishTrack(int tid, cgicc::Cgicc &cgi);
std::string updateCategories(int tid, cgicc::Cgicc &cgi);

// upload.cpp
std::string uploadTrack(int tid, cgicc::Cgicc &cgi);
std::string newTrack(cgicc::Cgicc &cgi);

// comment.cpp
std::string postComment(Comment::Type type, int ref, cgicc::Cgicc &cgi);

// art.cpp
std::string uploadArt(int tid, cgicc::Cgicc &cgi);

// favorite.cpp
std::string follow(int uid, bool add, cgicc::Cgicc &cgi);
std::string favorite(int tid, bool add, cgicc::Cgicc &cgi);

}

#endif // ACTIONS_H
