#include "../db.h"
#include <string.h>
#include <iostream>

std::string escape(const std::string &str){
    std::string buf;
    for(std::string::const_iterator i=str.begin(); i!=str.end(); i++){
        if(*i == '\\') buf += "\\\\";
        else if(*i == '-') buf += "\\-";
        else if(*i == '\n');
        else if(*i == '\r');
        else buf += *i;
    }
    return buf;
}

int main(int argc, char **argv){
    DB::connect();

    bool all = false, id=false;
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "--all") == 0 || strcmp(argv[i], "-a") == 0) all = true;
        if(strcmp(argv[i], "--id") == 0 || strcmp(argv[i], "-i") == 0) id = true;
    }

    if(id){
        DB::Result r = DB::query( (std::string) "SELECT id FROM tracks" + (all?"":" WHERE visible='t'"));
        for(DB::Result::const_iterator i=r.begin(); i!=r.end(); i++)
            std::cout << (*i)[0] << std::endl;
    }

    else {
        DB::Result r = DB::query( (std::string) "SELECT tracks.user_id, users.name, tracks.id, tracks.title FROM tracks, users WHERE users.id = tracks.user_id " + (all?"":"AND visible='t' ") + "ORDER BY id ASC");
        for(DB::Result::const_iterator i=r.begin(); i!=r.end(); i++)
            std::cout << (*i)[0] << " " << escape((*i)[1]) << " - " << (*i)[2] << " " << (*i)[3] << std::endl;
    }

    DB::close();
    return 0;
}
