#ifndef HTML_H
#define HTML_H

#include <vector>
#include <iostream>
#include "../track.h"
#include "../user.h"

namespace Html{

// escape.cpp
std::string escape(const std::string &str);

// redirect.cpp
std::string redirect(const std::string &location);

// page.cpp
std::string header(std::string title=std::string(), int status=200);
std::string header(std::string title, const std::string &head, int status=200);
std::string footer();
std::string notFound();

// track.cpp
enum List { Standard, Compact, Edition };
std::string trackPage(const Track &track, const std::string error=std::string());
std::string trackList(const std::vector<Track> &tracks, List l=Standard);
std::string downloadFlac(int id);
std::string downloadMp3(int id);

// user.cpp
std::string userPage(const User &user, const std::string &msg);
std::string userList(const std::vector<User> &users);
std::string usersPage();
std::string artistsPage();

// home.cpp
std::string home();

// registration.cpp
std::string registrationForm();

}

#endif // HTML_H
