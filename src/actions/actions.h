#ifndef ACTIONS_H
#define ACTIONS_H

#include <cgicc/Cgicc.h>

namespace Action{

// registration.cpp
std::string registration(cgicc::Cgicc &cgi);

// login.cpp
std::string login(cgicc::Cgicc &cgi);
std::string logout();

// account.cpp
std::string account(cgicc::Cgicc &cgi);

// upload.cpp
std::string uploadTrack(int id, cgicc::Cgicc &cgi);

// track.cpp
std::string newTrack(cgicc::Cgicc &cgi);
std::string deleteTrack(int id, cgicc::Cgicc &cgi);
std::string renameTrack(int id, cgicc::Cgicc &cgi);
std::string trackVisibility(int id, cgicc::Cgicc &cgi);

}

#endif // ACTIONS_H
