#include <stdlib.h>
#include <arpa/inet.h>
#include <sstream>
#include "user.h"
#include "db.h"

User::User(int nId){
    _id = -1;
    if(nId<0) return;

    unsigned int uid = htonl(nId);
    const char* val[1] = { (char*) &uid };
    int len[1] = { sizeof(nId) };
    int bin[1] = { 1 };
    PGresult* result = PQexecParams(db, 
        "SELECT name FROM users WHERE id = $1",
        1, 0, val, len, bin, 0);

    if(PQntuples(result) != 0){
        _id = nId;
        _name = PQgetvalue(result, 0, 0);
    }

    PQclear(result);
} 

std::string User::pageUrl(int id){
    std::stringstream s;
    s << "/user/" << id;
    return s.str();
}

std::vector<User> User::listHelper(const char* format, unsigned int n, unsigned int begin){
    char query[100];
    sprintf(query, format, n, begin);
    PGresult* result = PQexec(db, query);

    int rows = PQntuples(result);
    std::vector<User> users(rows);
    for(int row=0; row<rows; row++)
        users[row] = User(atoi(PQgetvalue(result, row, 0)), PQgetvalue(result, row, 1));

    PQclear(result);
    return users;
}

std::vector<User> User::list(unsigned int n, unsigned int begin){
    return listHelper( 
        "SELECT id, name FROM users "
        "ORDER BY lower(name) ASC LIMIT %d OFFSET %d", n, begin);
}
std::vector<User> User::listArtists(unsigned int n, unsigned int begin){
    return listHelper( 
        "SELECT id, name FROM users WHERE EXISTS ( SELECT 1 FROM tracks WHERE user_id = users.id AND visible = 't' ) "
        "ORDER BY lower(name) ASC LIMIT %d OFFSET %d", n, begin);
}
