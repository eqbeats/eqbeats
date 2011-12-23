#include "html.h"

using namespace std;

#define REP(c,s) case (c):str.replace(i,++i,(s)); i=str.begin(); break;

/*std::string Html::escape(std::string str) {
    for(string::iterator i=str.begin(); i!=str.end(); i++){
        switch(*i){
            case '<' :
                str.replace(i, i, "&lt;");
                i=str.begin()
                break;
            case '>' :
                str.replace(i, i, "&gt;");
                break;
            case '&' :
                str.replace(i, i, "&amp;");
                break;
            REP('<', "&lt;")
            REP('>', "&gt;")
            //REP('&', "&amp;")
        }
    }
    return str;
}*/

string Html::escape(const string &str){
    string buf;
    for(string::const_iterator i=str.begin(); i!=str.end(); i++){
        if(*i == '<') buf += "&lt;";
        else if(*i == '>') buf += "&gt;";
        else if(*i == '&') buf += "&amp;";
        else if(*i == '"') buf += "&quot;";
        else buf += *i;
    }
    return buf;
}
