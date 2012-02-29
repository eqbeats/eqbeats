#include "media.h"
#include "number.h"
#include "path.h"
#include "user.h"
#include <stdio.h>
#include <taglib/taglib.h>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>

std::string Media::filePath(Format f) const{
    string format = f==Vorbis? "ogg" : "mp3";
    return eqbeatsDir() + "/tracks/" + number(id()) + "." + format;
}

void Media::convertToVorbis(){
    if(fork() == 0){
        freopen("/dev/null","r",stdin);
        std::string logfile = eqbeatsDir() + "/ffmpeg.log";
        freopen(logfile.c_str(),"a",stdout);
        freopen(logfile.c_str(),"a",stderr);
        string base = eqbeatsDir() + "/tracks/" + number(id()) + ".";
        string mp3 = base + "mp3";
        string vorbis = base + "ogg";
        execlp("ffmpeg", "ffmpeg", "-loglevel", "quiet", "-y", "-i", mp3.c_str(), "-acodec", "libvorbis", vorbis.c_str(), NULL);
    }
}

void Media::updateTags(Format format){
    if(format == MP3){
        TagLib::MPEG::File mp3(filePath(MP3).c_str());
        TagLib::Tag *t = mp3.tag();
        if(!t) return;
        t->setTitle(TagLib::String(title(), TagLib::String::UTF8));
        t->setArtist(TagLib::String(artist().name(), TagLib::String::UTF8));
        mp3.save(TagLib::MPEG::File::ID3v1 | TagLib::MPEG::File::ID3v2);
    } else if(format == Vorbis) {
        TagLib::Ogg::Vorbis::File vorbis(filePath(Vorbis).c_str());
        TagLib::Tag *t = vorbis.tag();
        if(!t) return;
        t->setTitle(TagLib::String(title(), TagLib::String::UTF8));
        t->setArtist(TagLib::String(artist().name(), TagLib::String::UTF8));
        vorbis.save();
    }
}

void Media::unlink(){
    ::unlink(filePath(MP3).c_str());
    ::unlink(filePath(Vorbis).c_str());
}
