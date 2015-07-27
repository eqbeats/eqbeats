#include <eqbeats.h>
#include <core/db.h>
#include <track/audio.h>
#include <text/text.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv){
    if(argc != 2){
        cerr << "Usage: updatetags ID" << endl;
        return 1;
    }

    int id = number(argv[1]);
    if(!id){
        cerr << "invalid ID" << endl;
        return 1;
    }

    struct eqbeats_ctx eq;
    if(eqbeats_init(&eq, "", NULL, 0) != 0){
        cerr << "eqbeats initialization failed, check that the EQBEATS_POSTGRES environment variable is correct" << endl;
        return 1;
    }

    DB::connect("updatetags");
    DB::setPgDatabase(eq.pg);

    Track t(id);

    if (!t){
        cerr << "track does not exist" << endl;
        return 1;
    }

    Audio a(&t);
    a.updateTags();

    DB::close();
    return 0;
}
