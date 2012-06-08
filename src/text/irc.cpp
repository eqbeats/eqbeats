#include "modifiers.h"

const char* accepted = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789|-[]{}\\`^_";

void IrcEscape::Modify(const char *in, size_t inlen, const ctemplate::PerExpandData*, ctemplate::ExpandEmitter *out, const std::string&) const{
    size_t max = inlen > 20 ? 20 : inlen;
    for(size_t i=0; i<max; i++){
        if((in[i] & 0b10000000) == 0 || (in[i] & 0b01000000) != 0)
            out->Emit(strchr(accepted, in[i]) ? in[i] : '_');
    }
}
