#include "art.h"
#include "number.h"
#include "path.h"
#include <Magick++.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

Art::Art(int tid){
    _tid = tid;
    if(access(filepath().c_str(), R_OK))
        _tid = 0;
}

std::string Art::filepath(Art::Size sz) const{
    return eqbeatsDir() + "/art/" +
        (sz==Medium ? "medium/" : sz==Thumbnail ? "thumb/" : "") + number(_tid) + (sz==Full?"":".png");
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
    // TIFF: 0x49492a00
    if(magic[0] == 0xff && magic[1] == 0xd8) format = JPEG;
    if(magic[0] == 0x89 && magic[1] == 'P' &&
       magic[2] == 'N'  && magic[3] == 'G') format = PNG;
    if(magic[0] == 'G'  && magic[1] == 'I' &&
       magic[2] == 'F'  && magic[3] == '8') format = GIF;
    if(magic[0] == 'I'  && magic[1] == 'I' &&
       magic[2] == '*'  && magic[3] == 0) format = TIFF;
    fclose(f);
    return format;
}

void Art::makeThumbs(){
    if(_tid<=0) return;
    Magick::Image i;
    unlink(filepath(Medium).c_str());
    unlink(filepath(Thumbnail).c_str());
    try {
        try{ i.read(filepath()); }
        catch(Magick::Warning &warn){
            std::cerr << "ImageMagick Warning : " << warn.what() << std::endl;
        }
        Format f = getFormat();
        if(f!=GIF && (i.size().height()>480 || (f!=JPEG && f!=PNG))){
            if(i.size().height() > 480)
                i.scale("x480");
            i.write(filepath(Medium));
        }
        if(i.size().height() > 64)
            i.scale("x64");
        i.write(filepath(Thumbnail));
    } catch ( Magick::Exception &err ) {
        std::cerr << "ImageMagick Exceptions : " << err.what() << std::endl;
    }
}

void Art::remove(){
    if(_tid <= 0) return;
    unlink(filepath(Full).c_str());
    unlink(filepath(Medium).c_str());
    unlink(filepath(Thumbnail).c_str());
}
