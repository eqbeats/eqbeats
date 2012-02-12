#include "../db.h"
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/taglib.h>
#include <iostream>

using namespace std;
using namespace TagLib;

int main(int argc, char **argv){
    DB::connect();

    DB::Result r = DB::query( (std::string) "SELECT tracks.id, tracks.title, users.name FROM tracks, users WHERE users.id = tracks.user_id ORDER BY id ASC");

    for(DB::Result::const_iterator i=r.begin(); i!=r.end(); i++){
        cout << (*i)[0] << " " << (*i)[1] << endl;
        // MP3
        std::string mp3path = (*i)[0] + ".mp3";
        MPEG::File mp3(mp3path.c_str());
        Tag *tag = mp3.tag();
        tag->setTitle((*i)[1]);
        tag->setArtist((*i)[2]);
        mp3.save(MPEG::File::ID3v1 | MPEG::File::ID3v2);
        // Vorbis
        std::string oggpath = (*i)[0] + ".ogg";
        Ogg::Vorbis::File ogg(oggpath.c_str());
        Tag *oggtag = ogg.tag();
        oggtag->setTitle((*i)[1]);
        oggtag->setArtist((*i)[2]);
        ogg.save();
    }

    DB::close();
    return 0;
}
