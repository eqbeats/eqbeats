#ifndef ROUTING_H
#define ROUTING_H

#include <string>

// Returns 0 on error.
int routeUser(const std::string &url);
int routeTrack(const std::string &url);
int routeTrackAction(const std::string &act, const std::string &url);

#endif // ROUTING_H
