#ifndef HTML_H
#define HTML_H

#include <iostream>
#include "../track.h"
#include "../user.h"
#include "../comment.h"

extern std::string path;

//! Page generation
namespace Html{

// escape.cpp
std::string escape(const std::string &);
std::string escapeEmail(const std::string &);
std::string format(std::string);

// redirect.cpp
std::string redirect(const std::string &location);

// page.cpp
std::string header(const std::string &title=std::string(), int status=200);
std::string footer();
std::string quickStart();
std::string notFound(const std::string &what="Page");
std::string errorPage(const std::string &err);

enum TrackList {
    Standard,
    //! Artist not displayed.
    Compact,
    Edition
};

// track.cpp
std::string uploadForm(const std::string &action);
std::string trackPage(int tid);
std::string embedTrack(int tid);
std::string trackList(const std::vector<Track> &tracks, TrackList l=Standard);
std::string tracksPage();
std::string trackSearch(const std::string &q);
std::string latestTracks(int n);
std::string randomTracks(int n);
std::string popularTracks(int n);
std::string downloadTrack(int tid, Track::Format f);
std::string trackArt(int tid);

// user.cpp
std::string userList(const std::vector<User> &users);
std::string userPage(int uid);
std::string userSearch(const std::string &q);
std::string usersPage();
std::string artistsPage();
std::string searchForm(const std::string &action, const std::string &q=std::string());

// news.cpp
std::string newsPage(int nid);
std::string latestNews(int n);

// comment.cpp
std::string comments(const std::vector<Comment> &cmts, const std::string title="Comments");
std::string commentForm(const std::string &action);

// fac.cpp
std::string faq();

}

#endif // HTML_H
