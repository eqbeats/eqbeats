#ifndef HTML_H
#define HTML_H

#include <iostream>
#include "../track.h"
#include "../art.h"
#include "../user.h"
#include "../comment.h"
#include "render.h"
#include "html/escape.h"

//! Page generation
namespace Render{
namespace Html{

// page.cpp
void header(const std::string &title=std::string(), const std::string &head=std::string(), int status=200);
std::string atomFeed(const std::string &url);
void footer();
void quickStart();
void notFound(const std::string &what="Page");
void errorPage(const std::string &err);

enum TrackList {
    Standard,
    //! Artist not displayed.
    Compact,
};

// track.cpp
void uploadForm(const std::string &action);
void trackPage(int tid);
void embedTrack(int tid);
std::string embedTrackCode(const Track &t, int w=0);
void trackList(const std::vector<Track> &tracks, TrackList l=Standard);
void latestTracks(int n);
void tracksPage(const std::string &title, const std::vector<Track> &tracks);
void trackSearch(const std::string &q);
void category(int cid);
void downloadTrack(int tid, Track::Format f);
void trackArt(int tid, Art::Size sz=Art::Full);

// feed.cpp
void feedIcon(const std::string &url);

// user.cpp
void userList(const std::vector<User> &users);
void userPage(int uid);
void userSearch(const std::string &q);
void usersPage();
void artistsPage();
void searchForm(const std::string &action, const std::string &q=std::string());
void favorites(int uid);

// news.cpp
void newsPage(int nid);
void latestNews(int n);

// comment.cpp
void comments(const std::vector<Comment> &cmts, const std::string title="Comments");
void commentForm(const std::string &action);

// contest.cpp
void contest(int cid, const std::string &host);

// fac.cpp
void faq();

// credits.cpp
void credits();

// home.cpp
void home();

}
}

#endif // HTML_H
