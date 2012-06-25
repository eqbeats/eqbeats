PATH("/users/search"){
    std::string q = cgi("q");
    HTML(q);
    tpl = "html/userlist-page.tpl";
    dict->SetValue("TITLE", q);
    AccountList::search(q).fill(dict, "USERLIST");
}

PATH("/artists"){
    HTML("Artists");
    tpl = "html/userlist-page.tpl";
    dict->SetValue("TITLE", "Artists");
    int p = number(cgi("p"));
    if(p < 1) p = 1;
    AccountList l = AccountList::artists(21, 20*(p-1));
    if(p > 1)
        dict->SetValueAndShowSection("PREV", number(p-1), "HAS_PREV");
    if(l.size() == 20)
        dict->SetValueAndShowSection("NEXT", number(p+1), "HAS_NEXT");
    if(!l.empty()) l.pop_back();
    l.fill(dict, "USERLIST");
}

// JSON

{
AccountList json_users;
bool is_json_array = false;
PATH("/users/search/json")
    is_json_array = true, json_users = AccountList::search(cgi("q"));
PATH("/artists/json")
    is_json_array = true, json_users = AccountList::artists(500);
if(is_json_array){
    JSON();
    tpl = "json/array.tpl";
    Dict *item, *data;
    for(AccountList::const_iterator i=json_users.begin(); i!=json_users.end(); i++){
        item = dict->AddSectionDictionary("ITEM");
        data = item->AddIncludeDictionary("DATA");
        data->SetFilename("json/account.tpl");
        i->fill(data);
    }
}
}
