#include "media.h"
#include "number.h"
#include "path.h"
#include "user.h"
#include "log.h"
#include <stdio.h>
#include <sys/wait.h>
#include <taglib/taglib.h>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

Media::Media(const Track &t): Track(t){
    std::string path = eqbeatsDir() + "/tracks";
    DIR *dirp = opendir(path.c_str());
    struct dirent *e;
    std::string name = number(t.id()) + ".orig.";
    while((e = readdir(dirp)) != NULL){
        if(strncmp(e->d_name, name.c_str(), name.size()) == 0){
            ext = e->d_name + (name.size() - 1);
            break;
        }
    }
    closedir(dirp);
}

std::string Media::extension(Format f) const{
    return f == Vorbis ? ".ogg" : f == MP3 ? ".mp3" : ext;
}

std::string Media::filePath(Format f) const{
    return eqbeatsDir() + "/tracks/" + number(id()) + (f == Original ? ".orig" : "") + extension(f);
}

#define FORMAT(e,m) if(extension() == (e)) return (m);

std::string Media::mimetype(Format f) const{
    if(f == Track::Vorbis) return "audio/ogg";
    if(f == Track::MP3) return "audio/mpeg";
    FORMAT(".aiff", "audio/aiff")
    FORMAT(".flac", "audio/flac")
    FORMAT(".wav", "audio/wav")
    return "application/octet-stream";
}

void Media::convertToVorbis(){
    pid_t child = fork();
    if(child == 0){
        freopen("/dev/null","r",stdin);
        std::string logfile = eqbeatsDir() + "/ffmpeg.log";
        freopen(logfile.c_str(),"a",stdout);
        freopen(logfile.c_str(),"a",stderr);
        string base = eqbeatsDir() + "/tracks/" + number(id()) + ".";
        string mp3 = base + "mp3";
        string vorbis = base + "ogg";
        execlp("ffmpeg", "ffmpeg", "-loglevel", "quiet", "-y", "-i", mp3.c_str(), "-acodec", "libvorbis", vorbis.c_str(), NULL);
    }
    else if(child > 0){
        int stat;
        waitpid(child, &stat, 0);
        log("ffmpeg: exited with " + number(stat));
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
    ::unlink(filePath(Original).c_str());
}
