#include "escape.h"
#include "../../number.h"
#include <pcrecpp.h>

using namespace std;
using namespace Render;

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

string Html::escapeEmail(const string &email){
    string buf;
    for(string::const_iterator i=email.begin(); i!=email.end(); i++)
        buf += "&#" + number(*i) + ";";
    return buf;
}

std::string Html::format(std::string txt){
    txt = escape(txt);
    string tmp;
    for(string::const_iterator i=txt.begin(); i!=txt.end(); i++){
        if(*i == '\n') tmp += "<br />";
        else tmp += *i;
    }
    pcrecpp::RE("\\[b\\](.*?)\\[/b\\]").GlobalReplace("<b>\\1</b>", &tmp);
    pcrecpp::RE("\\[i\\](.*?)\\[/i\\]").GlobalReplace("<i>\\1</i>", &tmp);
    pcrecpp::RE("\\[u\\](.*?)\\[/u\\]").GlobalReplace("<u>\\1</u>", &tmp);
    pcrecpp::RE("(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?").GlobalReplace("<a href=\"\\0\">\\0</a>", &tmp);
    return tmp;
}
