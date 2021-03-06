#include "art.h"
#include <core/path.h>
#include <text/text.h>

#include <iostream>
#include <wand/magick_wand.h>
#include <stdio.h>
#include <unistd.h>

Art::Art(int tid){
    _tid = tid;
    if(access(filepath().c_str(), R_OK)) // file doesn't exist
        _tid = 0;
}

std::string Art::filepath(Art::Size sz) const{
    return eqbeatsDir() + "/art/" +
        (sz==Medium ? "medium/" : sz==Thumbnail ? "thumb/" : "") + number(_tid) + (sz==Full?"":sz==Medium?".jpg":".png");
}

std::string Art::url(Art::Size sz) const{
    return "/track/" + number(_tid) + "/art" +
        (sz==Medium ? "/medium" : sz==Thumbnail ? "/thumb" : "");
}

std::string getFormat(const char *filepath){
    FILE *f = fopen(filepath, "rb");
    std::string format = "";
    unsigned char magic[4];
    fread(&magic, 1, 4, f);
    // JPEG: 0xffd8
    // PNG:  0x89504e470d0a1a0a
    // GIF:  0x47494638
    // TIFF: 0x49492a00
    if(magic[0] == 0xff && magic[1] == 0xd8) format = "jpg";
    if(magic[0] == 0x89 && magic[1] == 'P' &&
       magic[2] == 'N'  && magic[3] == 'G') format = "png";
    if(magic[0] == 'G'  && magic[1] == 'I' &&
       magic[2] == 'F'  && magic[3] == '8') format = "gif";
    if(magic[0] == 'I'  && magic[1] == 'I' &&
       magic[2] == '*'  && magic[3] == 0) format = "tif";
    fclose(f);
    return format;
}

void Art::makeThumbs(){
    if(_tid<=0) return;
    // MagickWandGenesis();
    MagickWand *wand = NewMagickWand();
    if(!wand)
        return;
    // Remove previously existing thumbnails
    unlink(filepath(Medium).c_str());
    unlink(filepath(Thumbnail).c_str());
    // Magick
    if(!MagickReadImage(wand, filepath().c_str())){
        DestroyMagickWand(wand);
        return;
    }
    float aspect = ((float) MagickGetImageWidth(wand)) / ((float) MagickGetImageHeight(wand));
    std::string f = getFormat(filepath().c_str());
    if(f!="gif" && (MagickGetImageHeight(wand)>480 || (f!="jpg" && f!="png"))){
        // ^ Don't make a medium thumbnail for GIF (to preserve animation)
        // Otherwise make the thumbnail if the pic is large OR if the format isn't known
        if(MagickGetImageWidth(wand) > 1000) // scale in the first case
            MagickScaleImage(wand, 1000, 1000.0 / aspect);
        MagickSetImageCompressionQuality(wand, 90);
        MagickWriteImage(wand, filepath(Medium).c_str());
    }
    if(MagickGetImageHeight(wand) > 64) // resize (most of the time)
        MagickScaleImage(wand, 64 * aspect, 64);
    MagickWriteImage(wand, filepath(Thumbnail).c_str());
    if(wand)
        DestroyMagickWand(wand);
}

void Art::remove(){
    if(_tid <= 0) return;
    unlink(filepath(Full).c_str());
    unlink(filepath(Medium).c_str());
    unlink(filepath(Thumbnail).c_str());
}

File Art::thumbnail() const{
    return File("art/thumb/" + number(_tid) + ".png", "thumbnail.png");
}

File Art::medium() const{
    if(access(filepath(Medium).c_str(), R_OK)) // medium thumbnail doesn't exist
        return full();
    return File("art/medium/" + number(_tid) + ".jpg", "medium.jpg");
}

File Art::full() const{
    std::string f = getFormat(filepath().c_str());
    return File("art/" + number(_tid), "cover" + (f.empty() ? "" : "." + f));
}
