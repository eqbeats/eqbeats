#include <core/db.h>
#include <iostream>

std::string show(const std::string &q){
    DB::Result r = DB::query(q);
    if(r.empty()) return std::string();
    return r[0][0];
}

int main(){
    DB::connect();
    std::cout << show("SELECT count(*) FROM tracks") << " tracks ("
              << show("SELECT count(*) FROM tracks WHERE visible='f'") << " hidden)" << std::endl
              << show("SELECT count(*) FROM playlists") << " playlists" << std::endl
              << show("SELECT count(*) FROM users") << " users" << std::endl
              << show("SELECT count(*) FROM users WHERE EXISTS ( SELECT 1 FROM tracks WHERE user_id = users.id AND visible = 't' )") << " artists" << std::endl
              << "Last user: " << show("SELECT name FROM users ORDER BY registration DESC LIMIT 1") << std::endl;
    DB::close();
    return 0;
}
