#include "../number.h"
#include "../art.h"
#include <iostream>

int main(int argc, char **argv){
    if(argc != 2){
        std::cerr << "Usage: mkthumbs ID" << std::endl;
        return 1;
    }
    int tid = number(argv[1]);
    if(!tid) return 1;
    Art art(tid);
    if(!art) return 1;
    art.makeThumbs();
    return 0;
}
