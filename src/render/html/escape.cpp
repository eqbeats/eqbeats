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

string Html::format(string txt){
    txt = escape(txt);
    pcrecpp::RE("\r?\n").GlobalReplace("<br />", &txt);
    pcrecpp::RE("\\[b\\](.*?)\\[/b\\]").GlobalReplace("<b>\\1</b>", &txt);
    pcrecpp::RE("\\[i\\](.*?)\\[/i\\]").GlobalReplace("<i>\\1</i>", &txt);
    pcrecpp::RE("\\[u\\](.*?)\\[/u\\]").GlobalReplace("<u>\\1</u>", &txt);
    pcrecpp::RE("(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?").GlobalReplace("<a href=\"\\0\" rel=\"nofollow\">\\0</a>", &txt);
    return txt;
}

string Html::strip(string txt){
    pcrecpp::RE("\\.?[ \r?\n]*\n").GlobalReplace(". ", &txt);
    pcrecpp::RE("\\[/?[uib]\\]").GlobalReplace("", &txt);
    return txt;
}
