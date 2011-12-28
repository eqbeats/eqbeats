#include <sstream>
#include <stdlib.h>

using namespace std;

bool isNumber(const std::string &str){
    if(str.empty()) return false;
    bool num = true;
    for(std::string::const_iterator i=str.begin(); i!=str.end(); i++)
        num = num && ('0' <= *i && *i <= '9');
    return num;
}

string number(int n){
    stringstream s;
    s << n;
    return s.str();
}

int number(const std::string &str){
    return isNumber(str) ? atoi(str.c_str()) : 0;
}

string eqbeatsDir(){
    return (std::string) getenv("EQBEATS_DIR");
}
