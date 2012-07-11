#include "list.h"
#include <text/text.h>

#include <stdio.h>

#define FIELDS "users.id, users.name, users.email, users.about"

AccountList::AccountList(const std::string &query_fmt){
    char *query = (char*) malloc(query_fmt.size() + strlen(FIELDS));
    sprintf(query, query_fmt.c_str(), FIELDS);
    extract(DB::query(query));
    free(query);
}

void AccountList::extract(const DB::Result &r){
    resize(r.size());
    for(unsigned i=0; i<r.size(); i++){
        at(i).id = number(r[i][0]);
        at(i).name = r[i][1];
        at(i).email = r[i][2];
        at(i).about = r[i][3];
    }
}

Dict* AccountList::fill(Dict *d, const std::string &marker){
    Dict *list_d = d->AddIncludeDictionary(marker);
    list_d->SetFilename("html/userlist.tpl");
    if(empty()){
        list_d->ShowSection("EMPTY");
        return list_d;
    }
    for(const_iterator i=begin(); i!=end(); i++)
        i->fill(list_d->AddSectionDictionary("USER"));
    return list_d;
}

// Prepared queries

AccountList AccountList::all(int n, int offset){
    return AccountList(
        "SELECT %s FROM users ORDER BY name ASC "
        "LIMIT " + number(n) + " OFFSET " + number(offset));
}

AccountList AccountList::artists(int n, int offset){
    return AccountList(
        "SELECT %s FROM users WHERE EXISTS "
        "(SELECT 1 FROM tracks WHERE user_id = users.id AND visible='t') "
        "ORDER BY name ASC LIMIT " + number(n) + " OFFSET " + number(offset));
}

AccountList AccountList::search(const std::string &q){
    AccountList list;
    list.extract(DB::query("SELECT %s FROM WHERE name ILIKE $1 ORDER BY name ASC", "%"+q+"%"));
    return list;
}

