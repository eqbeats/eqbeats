#include <core/db.h>
#include <core/path.h>
#include <text/text.h>
#include <iostream>
#include <taglib/mpegfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/mp4file.h>
#include <taglib/opusfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/taglib.h>
#include <taglib/tpropertymap.h>

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
        String t((*i)[1], String::UTF8), a((*i)[2], String::UTF8);
        // MP3
        std::string mp3path = (string) dir + "/tracks/" + (*i)[0] + ".mp3";
        MPEG::File mp3(mp3path.c_str());
        Tag *tag = mp3.tag();
        tag->setTitle(t);
        tag->setArtist(a);
        mp3.save(MPEG::File::ID3v1 | MPEG::File::ID3v2);
        // Vorbis
        std::string oggpath = (string) dir + "/tracks/" + (*i)[0] + ".ogg";
        Ogg::Vorbis::File ogg(oggpath.c_str());
        Tag *oggtag = ogg.tag();
        if(oggtag != NULL){
            oggtag->setTitle(t);
            oggtag->setArtist(a);
        } else {
            Ogg::XiphComment comment;
            comment.setTitle(t);
            comment.setArtist(a);
            ogg.setProperties(comment.properties());
        }
        ogg.save();
        // AAC
        std::string aacpath = (string) dir + "/tracks/" + (*i)[0] + ".m4a";
        MP4::File aac(aacpath.c_str());
        TagLib::Tag *aactag = aac.tag();
        if(aactag != NULL){
            aactag->setTitle(t);
            aactag->setArtist(a);
        } else {
            PropertyMap map;
            map.insert("TITLE", t);
            map.insert("ARTIST", a);
            aac.setProperties(map);
        }
        aac.save();
        // Opus
        std::string opuspath = (string) dir + "/tracks/" + (*i)[0] + ".opus";
        Ogg::Opus::File opus(opuspath.c_str());
        Tag *opustag = opus.tag();
        if(opustag != NULL){
            opustag->setTitle(t);
            opustag->setArtist(a);
        } else {
            PropertyMap map;
            map.insert("TITLE", t);
            map.insert("ARTIST", a);
            opus.setProperties(map);
        }
        opus.save();
    }

    DB::close();
    return 0;
}
