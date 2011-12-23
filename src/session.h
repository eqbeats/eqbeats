#ifndef SESSION_H
#define SESSION_H

#include <cgicc/Cgicc.h>
#include "user.h"

namespace Session{

void start(cgicc::Cgicc &cgi);
void destroy();

User user();

// Returns the session's id or an empty string.
std::string login(const std::string &email, const std::string &pw, const std::string &host);
std::string login(int id, const std::string &host);

void logout();

}

#endif // SESSION_H
