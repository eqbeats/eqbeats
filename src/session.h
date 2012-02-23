#ifndef SESSION_H
#define SESSION_H

#include <string>

class User;

namespace Session{

//! \name Session management
//@{

/*! \brief Initialize the session.
 *
 * Reads the \c sid cookie and checks the \c sessions table.
 */
void start();

//! Reset the cached user.
void destroy();

//! Logged user.
User user();

//@}

//! \name Logging in/out
//@{

/*!
 * Creates a session in the \c sessions table.
 * \return The created session's id or an empty string.
 * \note The \c sid cookie must be sent to the client.
 */
std::string login(const std::string &email, const std::string &pw, const std::string &host);

//! \brief Trusted login.
std::string login(int id, const std::string &host);

/*!
 * Deletes the session from the database and unsets the \c sid cookie.
 */
void logout();

//@}

}

#endif // SESSION_H
