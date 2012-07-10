#include "pages.h"
#include "../list.h"
#include <core/cgi.h>
#include <text/text.h>

namespace Pages {

void users(Document *doc){

    if(path == "/users/search"){
        std::string q = cgi("q");
        doc->setHtml("html/userlist-page.tpl", q);
        doc->dict()->SetValue("TITLE", q);
        AccountList::search(q).fill(doc->dict(), "USERLIST");
    }

    if(path == "/artists"){
        doc->setHtml("html/userlist-page.tpl", "Artists");
        doc->dict()->SetValue("TITLE", "Artists");
        int p = number(cgi("p"));
        if(p < 1) p = 1;
        AccountList l = AccountList::artists(21, 20*(p-1));
        if(p > 1)
            doc->dict()->SetValueAndShowSection("PREV", number(p-1), "HAS_PREV");
        if(l.size() == 20)
            doc->dict()->SetValueAndShowSection("NEXT", number(p+1), "HAS_NEXT");
        if(!l.empty()) l.pop_back();
        l.fill(doc->dict(), "USERLIST");
    }

// JSON

    AccountList json_users;
    bool is_json_array = false;
    if(path == "/users/search/json")
        is_json_array = true, json_users = AccountList::search(cgi("q"));
    if(path == "/artists/json")
        is_json_array = true, json_users = AccountList::artists(500);
    if(is_json_array){
        doc->setJson("json/array.tpl");
        Dict *item, *data;
        for(AccountList::const_iterator i=json_users.begin(); i!=json_users.end(); i++){
            item = doc->dict()->AddSectionDictionary("ITEM");
            data = item->AddIncludeDictionary("DATA");
            data->SetFilename("json/account.tpl");
            i->fill(data);
        }
    }

}

}
