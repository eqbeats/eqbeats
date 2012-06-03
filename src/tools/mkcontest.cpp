#include <core/db.h>
#include <misc/number.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv){
    if(argc != 2){
        cerr << "Usage: mkcontest (TITLE|ID) < ..." << endl;
        return 1;
    }
    std::string arg = argv[1];
    if(arg.empty()) return 1;

    DB::connect();

    std::string contents;
    cin.seekg(0, std::ios::end);
    contents.reserve(cin.tellg());
    cin.seekg(0, std::ios::beg);
    contents.assign(std::istreambuf_iterator<char>(cin),
                    std::istreambuf_iterator<char>());

    if(isNumber(arg))
        DB::query("UPDATE contests SET description = $1 WHERE id = $2", contents, arg);

    else{
        DB::Result r = DB::query("INSERT INTO contests (name, description) VALUES ($1, $2) RETURNING id", arg, contents);
        if(r.empty()){
            cerr << "Fail." << endl;
            return 1;
        }
        cout << "Contest id: " << r[0][0] << endl;
    }

    DB::close();
    return 0;
}
