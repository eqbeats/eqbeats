#include "modifiers.h"
#include <pcrecpp.h>

void Formatter::Modify(const char *in, size_t inlen, const ctemplate::PerExpandData*, ctemplate::ExpandEmitter *out, const std::string&) const{
    std::string txt;
    ctemplate::StringEmitter buf(&txt);
    ctemplate::pre_escape.Modify(in, inlen, 0, &buf, "");
    pcrecpp::RE("\r?\n").GlobalReplace("<br />", &txt);
    pcrecpp::RE("\\[b\\](.*?)\\[/b\\]").GlobalReplace("<b>\\1</b>", &txt);
    pcrecpp::RE("\\[i\\](.*?)\\[/i\\]").GlobalReplace("<i>\\1</i>", &txt);
    pcrecpp::RE("\\[u\\](.*?)\\[/u\\]").GlobalReplace("<u>\\1</u>", &txt);
    pcrecpp::RE("(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?").GlobalReplace("<a href=\"\\0\" rel=\"nofollow\">\\0</a>", &txt);
    out->Emit(txt);
}
