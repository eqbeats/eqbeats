#ifndef STAT_H
#define STAT_H

#include <string>

namespace Stat {

void push(std::string type, int uid, int tid = 0);

std::string getTrackJson(int tid);
std::string getUserJson(int uid);

}

#endif // STAT_H
