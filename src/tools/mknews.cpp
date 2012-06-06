#include <core/db.h>
#include <text/text.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv){
    if(argc != 2){
        cerr << "Usage: mknews (TITLE|ID) < ..." << endl;
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
        DB::query("UPDATE news SET contents = $1 WHERE id = $2", contents, arg);

    else{
        DB::Result r = DB::query("INSERT INTO news (title, contents, date) VALUES ($1, $2, 'now') RETURNING id", arg, contents);
        if(r.empty()){ // possible ?
            cerr << "Fail." << endl;
            return 1;
        }
        DB::query("INSERT INTO ticker (title, url, expire) VALUES ($1, $2, 'now'::timestamptz + '14d') RETURNING id", arg, "/news/" + r[0][0]);
        cout << "News id: " << r[0][0] << endl;
    }

    DB::close();
    return 0;
}
