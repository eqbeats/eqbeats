#ifndef HTML_H
#define HTML_H

#include <iostream>
#include "../track.h"
#include "../user.h"
#include "../comment.h"

extern std::string path;

//! Page generation
namespace Html{

//! \defgroup pages Pages

//! \name Escaping
//@{
// escape.cpp
std::string escape(const std::string &);
std::string escapeEmail(const std::string &);
std::string format(std::string);
//@}

// redirect.cpp
std::string redirect(const std::string &location);

// page.cpp
std::string header(const std::string &title=std::string(), int status=200);
std::string footer();

/*! \brief /quickstart
 * \ingroup pages
 */
std::string quickStart();

//! \name Errors
//@{
//! \ingroup pages
std::string notFound(const std::string &what="Page");
//! \ingroup pages
std::string errorPage(const std::string &err);
//@}

enum TrackList {
    Standard,
    //! Artist not displayed.
    Compact,
    Edition
};
//! \name Track
//@{
// track.cpp
std::string uploadForm(const std::string &action);

/*! \brief /track/TID
 * \ingroup pages
 */
std::string trackPage(int tid);

/*! \brief /track/TID/embed
 * \ingroup pages
 */
std::string embedTrack(int tid);

std::string trackList(const std::vector<Track> &tracks, TrackList l=Standard);

/*! \brief /tracks
 * \ingroup pages
 */
std::string tracksPage();

/*! \brief /tracks/search
 * \ingroup pages
 */
std::string trackSearch(const std::string &q);

/*! \brief /tracks/latest
 * \ingroup pages
 */
std::string latestTracks(int n);

/*! \brief /tracks/random
 * \ingroup pages
 */
std::string randomTracks(int n);

/*! \brief /tracks/popular
 * \ingroup pages
 */
std::string popularTracks(int n);

/*! \brief /track/TID/{flac,vorbis,mp3}
 * \ingroup pages
 */
std::string downloadTrack(int tid, Track::Format f);

//@}

//! \name User
//@{
// user.cpp

std::string userList(const std::vector<User> &users);

/*! \brief /user/UID
 * \ingroup pages
 */
std::string userPage(int uid);

/*! \brief /users/search
 * \ingroup pages
 */
std::string userSearch(const std::string &q);

/*! \brief /users
 *
 * List users.
 * \ingroup pages
 */
std::string usersPage();

/*! \brief /artists
 *
 * List artists (users that uploaded tracks).
 * \ingroup pages
 */
std::string artistsPage();
//@}
std::string searchForm(const std::string &action, const std::string &q=std::string());


//! \name News
//@{
// news.cpp

/*! \brief /news/NID
 * \ingroup pages
 */
std::string newsPage(int nid);

std::string latestNews(int n);

//@}

//! \name Comments
//@{
// comment.cpp

std::string comments(const std::vector<Comment> &cmts, const std::string title="Comments");

std::string commentForm(const std::string &action);

//@}

/*! \brief /faq
 * \ingroup pages
 */
std::string faq();

}

#endif // HTML_H
