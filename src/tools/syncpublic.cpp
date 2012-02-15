#include "../db.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

    if(argc != 2){
        cerr << "Usage: syncpublic DIR" << endl;
        return 1;
    }
    string prefix = argv[1];

    DB::connect();
    DB::Result r = DB::query("SELECT id FROM tracks WHERE visible='t'");

    string id, path, link;
    for(DB::Result::const_iterator i=r.begin(); i!=r.end(); i++){
        id = (*i)[0];
        link = id + ".mp3";
        path = prefix + "/" + link;
        symlink(path.c_str(), link.c_str());
    }

    DB::close();
    return 0;
}
