#include <core/db.h>
#include <core/path.h>
#include <text/text.h>
#include <iostream>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/taglib.h>

using namespace std;
using namespace TagLib;

int main(int argc, char **argv){

    const char *dir = getenv("EQBEATS_DIR");
    if(!dir){
        cerr << "EQBEATS_DIR not set." << endl;
        return 1;
    }

    if(argc != 2){
        cerr << "Usage: updatetags ID" << endl;
        return 1;
    }

    int id = number(argv[1]);
    if(!id){
        cerr << "invalid ID" << endl;
        return 1;
    }

    DB::connect();

    DB::Result r = DB::query("SELECT tracks.id, tracks.title, users.name FROM tracks, users WHERE users.id = tracks.user_id AND tracks.id = $1", number(id));

    for(DB::Result::const_iterator i=r.begin(); i!=r.end(); i++){
        cout << (*i)[0] << " " << (*i)[1] << endl;
        // MP3
        std::string mp3path = (string) dir + "/tracks/" + (*i)[0] + ".mp3";
        MPEG::File mp3(mp3path.c_str());
        Tag *tag = mp3.tag();
        tag->setTitle((*i)[1]);
        tag->setArtist((*i)[2]);
        mp3.save(MPEG::File::ID3v1 | MPEG::File::ID3v2);
        // Vorbis
        std::string oggpath = (string) dir + "/tracks/" + (*i)[0] + ".ogg";
        Ogg::Vorbis::File ogg(oggpath.c_str());
        Tag *oggtag = ogg.tag();
        oggtag->setTitle((*i)[1]);
        oggtag->setArtist((*i)[2]);
        ogg.save();
    }

    DB::close();
    return 0;
}
