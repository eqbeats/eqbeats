#include "modifiers.h"
#include "text.h"

void EmailEscape::Modify(const char *in, size_t inlen, const ctemplate::PerExpandData*, ctemplate::ExpandEmitter *out, const std::string&) const{
    for(size_t i=0; i<inlen; i++)
        out->Emit("&#" + number(in[i]) + ";");
}
