#include "list.h"
#include <text/text.h>

#define SEL "SELECT id, name, email, about FROM users "

AccountList::AccountList(const DB::Result &r){
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
    list_d->SetFilename("userlist.tpl");
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
    return AccountList(DB::query(SEL "ORDER BY name ASC LIMIT " + number(n) + " OFFSET " + number(offset)));
}

AccountList AccountList::artists(int n, int offset){
    return AccountList(DB::query(SEL "WHERE EXISTS (SELECT 1 FROM tracks WHERE user_id = users.id AND visible='t') ORDER BY name ASC LIMIT " + number(n) + " OFFSET " + number(offset)));
}

AccountList AccountList::search(const std::string &q){
    return AccountList(DB::query(SEL "WHERE name ILIKE $1 ORDER BY name ASC", "%"+q+"%"));
}

