#include "track.h"
#include "utils.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

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

void Track::convertToVorbis(){
    waitZombies();
    pid_t pid = fork();
    if(pid == 0){
        freopen("/dev/null","r",stdin);
        freopen("/tmp/ffmpeg.log","a",stdout);
        freopen("/tmp/ffmpeg.log","a",stderr);
        string base = eqbeatsDir() + "/tracks/" + number(id()) + ".";
        string mp3 = base + "mp3";
        string vorbis = base + "ogg";
        execlp("ffmpeg", "ffmpeg", "-y", "-i", mp3.c_str(), "-acodec", "libvorbis", vorbis.c_str(), NULL);
    }
    running.push_back(pid);
}
