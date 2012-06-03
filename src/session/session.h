#ifndef SESSION_H
#define SESSION_H

#include <core/template.h>

class User;

namespace Session{

// Reads the sid cookie.
void start();

// Resets the cached user.
void destroy();

// Logged user.
User user();

// Returns the sid. Doesn't set the cookie.
std::string login(const std::string &email, const std::string &pw);

// Trusted login.
std::string login(int id, const std::string &host);

void logout();

std::string randomString();

void fill(Dict *d);

}

#endif // SESSION_H
