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

void Track::convert(Format f){
    if(f == FLAC) return;
    waitZombies();
    pid_t pid = fork();
    if(pid == 0){
        freopen("/dev/null","r",stdin);
        freopen("/tmp/ffmpeg.log","a",stdout);
        freopen("/tmp/ffmpeg.log","a",stderr);
        string base = eqbeatsDir() + "/tracks/" + number(id()) + ".";
        string flac = base + "flac";
        string out = base + (f==Vorbis?"ogg":"mp3");
        execlp("ffmpeg", "ffmpeg", "-y", /*"-v", "warning",*/ "-i", flac.c_str(), 
               /*"-ab", "128k",*/"-acodec", (f==Vorbis?"libvorbis":"libmp3lame"), out.c_str(), NULL);
    }
    running.push_back(pid);
}
