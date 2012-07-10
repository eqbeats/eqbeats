#include "modifiers.h"
#include "text.h"
#include <pcrecpp.h>

bool validEmail(const std::string &email){
    return pcrecpp::RE("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?", pcrecpp::RE_Options().set_caseless(true)).FullMatch(email);
}

void EmailEscape::Modify(const char *in, size_t inlen, const ctemplate::PerExpandData*, ctemplate::ExpandEmitter *out, const std::string&) const{
    for(size_t i=0; i<inlen; i++)
        out->Emit("&#" + number(in[i]) + ";");
}
