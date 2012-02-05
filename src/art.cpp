#include "art.h"
#include "utils.h"
#include <Magick++.h>
#include <stdio.h>
#include <unistd.h>

Art::Art(int tid){
    _tid = tid;
    if(access(filepath().c_str(), R_OK))
        _tid = 0;
}

std::string Art::filepath(Art::Size sz) const{
    return eqbeatsDir() + "/art/" + 
        (sz==Medium ? "medium/" : sz==Thumbnail ? "thumb/" : "") + number(_tid);
}

std::string Art::url(Art::Size sz) const{
    return "/track/" + number(_tid) + "/art" +
        (sz==Medium ? "/medium" : sz==Thumbnail ? "/thumb" : "");
}

Art::Format Art::getFormat(){
    FILE *f = fopen(filepath().c_str(), "rb");
    Format format = Unknown;
    unsigned char magic[4];
    fread(&magic, 1, 4, f);
    // JPEG: 0xffd8
    // PNG:  0x89504e470d0a1a0a
    // GIF:  0x47494638
    if(magic[0] == 0xff && magic[1] == 0xd8) format = JPEG;
    if(magic[0] == 0x89 && magic[1] == 'P' &&
       magic[2] == 'N'  && magic[3] == 'G') format = PNG;
    if(magic[0] == 'G'  && magic[1] == 'I' &&
       magic[2] == 'F'  && magic[3] == '8') format = GIF;
    fclose(f);
    return format;
}

void Art::makeThumbs(){
    if(_tid<=0) return;
    Magick::Image i;
    unlink(filepath(Medium).c_str());
    symlink(("../" + number(_tid)).c_str(), filepath(Medium).c_str());
    unlink(filepath(Thumbnail).c_str());
    symlink(("../" + number(_tid)).c_str(), filepath(Thumbnail).c_str());
    try {
        i.read(filepath());
        if(getFormat() == JPEG && i.quality() > 90)
            i.quality(90);
        if(i.size().height() > 480){
            i.scale("x480");
            unlink(filepath(Medium).c_str());
            i.write(filepath(Medium));
        }
        if(i.size().height() > 64){
            i.scale("x64");
            unlink(filepath(Thumbnail).c_str());
            i.write(filepath(Thumbnail));
        }
    } catch ( Magick::Exception &err ) {
        return;
    }
}

void Art::remove(){
    if(_tid <= 0) return;
    unlink(filepath(Full).c_str());
    unlink(filepath(Medium).c_str());
    unlink(filepath(Thumbnail).c_str());
}
