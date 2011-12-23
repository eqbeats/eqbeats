#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

// FLAC tester

bool isFlac(const char *filename){
    //ifstream f(filename);
    char sig[4];
    FILE *f = fopen(filename, "r");
    //f.get(sig, 5);
    fread(sig, 1, 4, f);
    fclose(f);
    return !memcmp(sig, "fLaC", 4);
}

int main(int argc, char **argv){
    if(argc < 2) return 1;
    if(!isFlac(argv[1]))
        cout << "not ";
    cout << "flac." << endl;
    return 0;
}
