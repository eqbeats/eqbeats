#include "audio.h"
#include <core/path.h>
#include <text/text.h>

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <taglib/mpegfile.h>
#include <taglib/taglib.h>
#include <taglib/vorbisfile.h>
#include <unistd.h>

std::string Audio::filepath(Format f) const{
    std::string base = eqbeatsDir() + "/tracks/";
    if(f == MP3)    return base + number(track->id) + ".mp3";
    if(f == Vorbis) return base + number(track->id) + ".ogg";
    // Otherwise, Original
    DIR *dirp = opendir(base.c_str());
    struct dirent *e;
    std::string name = number(track->id) + ".orig.";
    std::string ext;
    while((e = readdir(dirp)) != NULL){
        if(strncmp(e->d_name, name.c_str(), name.size()) == 0){
            ext = e->d_name + name.size();
            break;
        }
    }
    closedir(dirp);
    if(ext.empty()) return std::string();
    return base + name + ext;
}

void Audio::updateTags(Format format){
    if(format == MP3){
        TagLib::MPEG::File mp3(filepath(MP3).c_str());
        TagLib::Tag *t = mp3.tag();
        if(!t) return;
        t->setTitle(TagLib::String(track->title, TagLib::String::UTF8));
        t->setArtist(TagLib::String(track->artist.name, TagLib::String::UTF8));
        mp3.save(TagLib::MPEG::File::ID3v1 | TagLib::MPEG::File::ID3v2);
    } else if(format == Vorbis) {
        TagLib::Ogg::Vorbis::File vorbis(filepath(Vorbis).c_str());
        TagLib::Tag *t = vorbis.tag();
        if(!t) return;
        t->setTitle(TagLib::String(track->title, TagLib::String::UTF8));
        t->setArtist(TagLib::String(track->artist.name, TagLib::String::UTF8));
        vorbis.save();
    }
}

void Audio::unlink(){
    ::unlink(filepath(MP3).c_str());
    ::unlink(filepath(Vorbis).c_str());
    ::unlink(filepath(Original).c_str());
}

File Audio::mp3() const{
    return File("tracks/" + number(track->id) + ".mp3", track->title + " - " + track->artist.name + ".mp3");
}

File Audio::vorbis() const{
    return File("tracks/" + number(track->id) + ".ogg", track->title + " - " + track->artist.name + ".ogg");
}

File Audio::original() const{
    std::string path = filepath(Original);
    std::string ext = path.substr(path.rfind('.'));
    return File("tracks/" + number(track->id) + ".orig" + ext, track->title + " - " + track->artist.name + ext);
}

void Audio::fill(Dict *d){
    std::string path = filepath(Original);
    // Status
    if(path.empty()){ // no original file -> not ready
        const char* status;
        if(access(filepath(MP3).c_str(), R_OK) == 0) // mp3 exists, but not the original file
            status = "Transcoding into MP3...";
        else if(access(filepath(Vorbis).c_str(), R_OK) == 0) // vorbis exists, but no mp3 nor original file
            status = "Transcoding into Vorbis...";
        else // no mp3, vorbis, or original file
            status = "Couldn't transcode.";
        d->SetValueAndShowSection("STATUS", status, "HAS_STATUS");
    }
    // Ready
    else{
        d->ShowSection("READY");
        std::string ext = path.substr(path.rfind('.'));
        if(ext == ".mp3") d->ShowSection("MP3_SOURCE");
        else d->SetValueAndShowSection("EXTENSION", ext, "OTHER_SOURCE");
    }
}
