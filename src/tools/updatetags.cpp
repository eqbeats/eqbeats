#include "../db.h"
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/xiphcomment.h>
#include <taglib/taglib.h>
#include <iostream>

using namespace std;
using namespace TagLib;

int main(int argc, char **argv){
    DB::connect();

    DB::Result r = DB::query( (std::string) "SELECT tracks.id, tracks.title, users.name FROM tracks, users WHERE users.id = tracks.user_id ORDER BY id ASC");

    for(DB::Result::const_iterator i=r.begin(); i!=r.end(); i++){
        cout << (*i)[0] << " " << (*i)[1] << endl;
        std::string mp3path = (*i)[0] + ".mp3";
        MPEG::File mp3(mp3path.c_str());
        // ID3v1
        ID3v1::Tag *v1tag = mp3.ID3v1Tag(true);
        v1tag->setTitle((*i)[1]);
        v1tag->setArtist((*i)[2]);
        // ID3v2
        ID3v2::Tag *v2tag = mp3.ID3v2Tag(true);
        v2tag->setTitle((*i)[1]);
        v2tag->setArtist((*i)[2]);
        mp3.save();
        // Vorbis
        std::string oggpath = (*i)[0] + ".ogg";
        Ogg::Vorbis::File ogg(oggpath.c_str());
        Ogg::XiphComment *oggtag = ogg.tag();
        if(!oggtag){
            cout << "OGG skipped." << endl;
            continue;
        }
        oggtag->setTitle((*i)[1]);
        oggtag->setArtist((*i)[2]);
        ogg.save();
    }

    DB::close();
    return 0;
}
