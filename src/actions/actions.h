#ifndef ACTIONS_H
#define ACTIONS_H

#include "../comment.h"
#include "../account.h"

//! Page generation with effects
namespace Action{

// login.cpp
void login();
void logout();

// password.cpp
void passwordReset();

// registration.cpp
void registration();
void account();
void accountForm(const Account &_account,
    const std::string &error=std::string(),
    const std::string &message=std::string(),
    const std::string &oldpw=std::string());

// track.cpp
void deleteTrack(int tid);
void renameTrack(int tid);
void updateNotes(int tid);
void publishTrack(int tid);
void setFlags(int tid);
void reportTrack(int tid);
void setTags(int tid);

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

// playlist.cpp
void createPlaylist();
void deletePlaylist(int pid);
void editPlaylist(int pid);
void playlistAdd(int tid);
void playlistRemove(int pid);
void playlistMove(int pid);

// youtube.cpp
void youtubeOauthCallback();
void youtubeUnlink();
void youtubeUpload(int tid);

}

#endif // ACTIONS_H
