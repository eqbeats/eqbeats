#include "audio.h"
#include <core/path.h>
#include <text/text.h>

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <taglib/mpegfile.h>
#include <taglib/taglib.h>
#include <taglib/vorbisfile.h>
#include <taglib/mp4file.h>
#include <taglib/opusfile.h>
#include <unistd.h>

std::string Audio::filepath(Format f) const{
    std::string base = eqbeatsDir() + "/tracks/";
    if(f == MP3)    return base + number(track->id) + ".mp3";
    if(f == Vorbis) return base + number(track->id) + ".ogg";
    if(f == AAC) return base + number(track->id) + ".aac";
    if(f == Opus)    return base + number(track->id) + ".opus";
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
    if(format == MP3 && access(filepath(MP3).c_str(), R_OK) == 0){
        TagLib::MPEG::File mp3(filepath(MP3).c_str());
        TagLib::Tag *t = mp3.tag();
        if(!t) return;
        t->setTitle(TagLib::String(track->title, TagLib::String::UTF8));
        t->setArtist(TagLib::String(track->artist.name, TagLib::String::UTF8));
        mp3.save(TagLib::MPEG::File::ID3v1 | TagLib::MPEG::File::ID3v2);
    } else if(format == Vorbis && access(filepath(Vorbis).c_str(), R_OK) == 0) {
        TagLib::Ogg::Vorbis::File vorbis(filepath(Vorbis).c_str());
        TagLib::Tag *t = vorbis.tag();
        if(!t) return;
        t->setTitle(TagLib::String(track->title, TagLib::String::UTF8));
        t->setArtist(TagLib::String(track->artist.name, TagLib::String::UTF8));
        vorbis.save();
    } else if(format == AAC && access(filepath(AAC).c_str(), R_OK) == 0) {
        TagLib::MP4::File aac(filepath(AAC).c_str());
        TagLib::Tag *t = aac.tag();
        if(!t) return;
        t->setTitle(TagLib::String(track->title));
        t->setArtist(TagLib::String(track->artist.name));
        aac.save();
    } else if(format == Opus && access(filepath(Opus).c_str(), R_OK) == 0) {
        TagLib::Ogg::Opus::File opus(filepath(Opus).c_str());
        TagLib::Tag *t = opus.tag();
        if(!t) return;
        t->setTitle(track->title);
        t->setArtist(track->artist.name);
        opus.save();
    }
}

void Audio::unlink(){
    ::unlink(filepath(MP3).c_str());
    ::unlink(filepath(Vorbis).c_str());
    ::unlink(filepath(Original).c_str());
    ::unlink(filepath(Opus).c_str());
    ::unlink(filepath(AAC).c_str());
}

File Audio::mp3() const{
    return File("tracks/" + number(track->id) + ".mp3",  track->artist.name + " - " + track->title + ".mp3");
}

File Audio::vorbis() const{
    return File("tracks/" + number(track->id) + ".ogg",  track->artist.name + " - " + track->title + ".ogg");
}

File Audio::aac() const{
    return File("tracks/" + number(track->id) + ".m4a",  track->artist.name + " - " + track->title + ".m4a");
}

File Audio::opus() const{
    return File("tracks/" + number(track->id) + ".opus", track->artist.name + " - " + track->title + ".opus");
}

File Audio::mood() const{
    return File("tracks/" + number(track->id) + ".mood", track->artist.name + " - " + track->title + ".mood");
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
        else if(access(filepath(Opus).c_str(), R_OK) == 0) // Vorbis, AAC and Opus exist
            status = "Transcoding into Opus...";
        else if(access(filepath(AAC).c_str(), R_OK) == 0) // Vorbis and AAC exist
            status = "Transcoding into AAC...";
        else if(access(filepath(Vorbis).c_str(), R_OK) == 0) // Only vorbis exists
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
