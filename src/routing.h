#ifndef ROUTING_H
#define ROUTING_H

#include <string>

// Returns 0 on error.
int routeId(const std::string &t, const std::string &path);
int routeAction(const std::string &t, const std::string &act, const std::string &path);

#endif // ROUTING_H
