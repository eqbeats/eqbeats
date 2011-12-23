#include <sstream>

using namespace std;

string number(int n){
    stringstream s;
    s << n;
    return s.str();
}
