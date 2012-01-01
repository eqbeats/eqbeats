#include "art.h"
#include "utils.h"
#include <stdio.h>

Art::Art(int tid){
    _tid = tid;
    if(access(filepath().c_str(), R_OK))
        _tid = 0;
}

std::string Art::filepath() const{
    return eqbeatsDir() + "/art/" + number(_tid);
}

std::string Art::url() const{
    return "/track/" + number(_tid) + "/art";
}

Art::Format Art::getFormat(){
    FILE *f = fopen(filepath().c_str(), "rb");
    Format format = Unknown;
    unsigned char magic[2];
    fread(&magic, 1, 2, f);
    // JPEG: 0xffd8
    // PNG:  0x89504e470d0a1a0a
    if(magic[0] == 0xff && magic[1] == 0xd8) format = JPEG;
    if(magic[0] == 0x89 && magic[1] == 0x50) format = PNG;
    fclose(f);
    return format;
}
