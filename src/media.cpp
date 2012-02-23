#include "media.h"
#include "number.h"
#include "path.h"
#include "user.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <taglib/taglib.h>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>

using namespace std;

vector<pid_t> running;

void waitZombies(){
    vector<pid_t> stillRunning;
    for(vector<pid_t>::const_iterator i=running.begin(); i!=running.end(); i++){
        if(!waitpid(*i, NULL, WNOHANG)) // process running
            stillRunning.push_back(*i);
    }
    running = stillRunning;
}

void Media::convertToVorbis(){
    waitZombies();
    pid_t pid = fork();
    if(pid == 0){
        freopen("/dev/null","r",stdin);
        std::string logfile = eqbeatsDir() + "/ffmpeg.log";
        freopen(logfile.c_str(),"a",stdout);
        freopen(logfile.c_str(),"a",stderr);
        //freopen("/dev/null","a",stdout);
        //freopen("/dev/null","a",stderr);
        string base = eqbeatsDir() + "/tracks/" + number(id()) + ".";
        string mp3 = base + "mp3";
        string vorbis = base + "ogg";
        execlp("ffmpeg", "ffmpeg", "-loglevel", "quiet", "-y", "-i", mp3.c_str(), "-acodec", "libvorbis", vorbis.c_str(), NULL);
    }
    running.push_back(pid);
}

void Media::updateTags(Track::Format format){
    if(format == MP3){
        TagLib::MPEG::File mp3(filePath(Track::MP3).c_str());
        TagLib::Tag *t = mp3.tag();
        if(!t) return;
        t->setTitle(title());
        t->setArtist(artist().name());
        mp3.save(TagLib::MPEG::File::ID3v1 | TagLib::MPEG::File::ID3v2);
    } else if(format == Vorbis) {
        TagLib::Ogg::Vorbis::File vorbis(filePath(Track::Vorbis).c_str());
        TagLib::Tag *t = vorbis.tag();
        if(!t) return;
        t->setTitle(title());
        t->setArtist(artist().name());
        vorbis.save();
    }
}
