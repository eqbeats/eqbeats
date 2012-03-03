#include "escape.h"
#include "../../number.h"
#include <pcrecpp.h>
#include <algorithm>

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
    string tmp;
    for(string::const_iterator i=txt.begin(); i!=txt.end(); i++){
        if(*i == '\n') tmp += "<br />";
        else tmp += *i;
    }
    pcrecpp::RE("\\[b\\](.*?)\\[/b\\]").GlobalReplace("<b>\\1</b>", &tmp);
    pcrecpp::RE("\\[i\\](.*?)\\[/i\\]").GlobalReplace("<i>\\1</i>", &tmp);
    pcrecpp::RE("\\[u\\](.*?)\\[/u\\]").GlobalReplace("<u>\\1</u>", &tmp);
    pcrecpp::RE("(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?").GlobalReplace("<a href=\"\\0\" rel=\"nofollow\">\\0</a>", &tmp);
    return tmp;
}

bool tag(string::const_iterator &beg, string::const_iterator &end){
    beg = find(beg, end, '[');
    string::const_iterator tagEnd = find(beg, end, ']');
    if(tagEnd != end){
        end = tagEnd+1;
        return true;
    }
    return false;
}

string Html::strip(const string &str){
    string::const_iterator beg = str.begin(), end = str.end();
    // Opening tag
    string::const_iterator open=beg, endOpen=end;
    if(!tag(open, endOpen)) return str;
    // Closing tag
    string::const_iterator close=endOpen, endClose=end;
    while(tag(close, endClose)){
        if(string(close+2, endClose) == string(open+1, endOpen)) // Tag closed
            return string(beg, open) + strip(string(endOpen, close)) + string(endClose, end);
        close = endClose;
        endClose = end;
    }
    return string(beg, endOpen) + strip(string(endOpen, end));
}
