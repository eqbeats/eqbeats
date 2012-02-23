#ifndef ACTIONS_H
#define ACTIONS_H

#include <cgicc/Cgicc.h>
#include "../comment.h"

//! Page generation with effects
namespace Action{

extern cgicc::Cgicc cgi;

// login.cpp
void login();
void logout();

// registration.cpp
void registration();
void account();

// track.cpp
void deleteTrack(int tid);
void renameTrack(int tid);
void updateNotes(int tid);
void publishTrack(int tid);
void updateCategories(int tid);
void reportTrack(int tid);

// upload.cpp
void uploadTrack(int tid);
void newTrack();

// comment.cpp
void postComment(Comment::Type type, int ref);

// art.cpp
void uploadArt(int tid);

// favorite.cpp
void follow(int uid, bool add);
void favorite(int tid, bool add);
void vote(int cid);

// contest.cpp
void contestSubmission(int cid);

}

#endif // ACTIONS_H
