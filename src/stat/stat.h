#ifndef STAT_H
#define STAT_H

#include <string>
#include <map>

using namespace std;

namespace Stat {

int push(string type, int uid, int tid = 0); // returns the tally of events of this type on this tid/uid

int get(const string type, int uid, int tid, bool unique = false);
map<string, int> getDays(const string type, int tid, bool unique = false);
map<string, int> getReferrers(int tid, int limit = 20);

}

#endif // STAT_H
