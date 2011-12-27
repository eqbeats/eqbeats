#ifndef ACTIONS_H
#define ACTIONS_H

#include <cgicc/Cgicc.h>

//! Page generation with effects
namespace Action{

//! \name Loggin in/out
//@{
// login.cpp

/*! \brief /login
 * \ingroup pages
 */
std::string login(cgicc::Cgicc &cgi);
//! \ingroup pages
/*! \brief /logout
 * \ingroup pages
 */
std::string logout(cgicc::Cgicc &cgi);

//@}

//! \name Account management
//@{

// registration.cpp
/*! \brief /register
 * \ingroup pages
 */
std::string registration(cgicc::Cgicc &cgi);

// account.cpp
/*! \brief /account
 * \ingroup pages
 */
std::string account(cgicc::Cgicc &cgi);

//@}

//! \name Actions on tracks
//@{
// track.cpp
/*! \brief /track/TID/delete
 * \ingroup pages
 */
std::string deleteTrack(int tid, cgicc::Cgicc &cgi);

/*! \brief /track/TID/rename
 * \ingroup pages
 */
std::string renameTrack(int tid, cgicc::Cgicc &cgi);

/*! \brief /track/TID/notes
 *
 * Update notes.
 * \ingroup pages
 */
std::string updateNotes(int tid, cgicc::Cgicc &cgi);

/*! \brief /track/TID/visibility
 * \ingroup pages
 */
std::string trackVisibility(int tid, cgicc::Cgicc &cgi);

/*! \brief /track/TID/comment
 * \ingroup pages
 */
std::string postComment(int tid, cgicc::Cgicc &cgi);

//@}

// upload.cpp
//! \name Track uploading
//@{

/*! \brief /track/TID/upload
 * \ingroup pages
 */
std::string uploadTrack(int tid, cgicc::Cgicc &cgi);

/*! \brief /track/new
 * \ingroup pages
 */
std::string newTrack(cgicc::Cgicc &cgi);

//@}


}

#endif // ACTIONS_H
