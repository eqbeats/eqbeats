ROUTE("user"){

Account u(id);
if(u){

SUB(""){
    HTML(u.name);
    tpl = "user.tpl";
    u.fill(dict);
}

SUB("comment");
SUB("follow");
SUB("unfollow");
SUB("favorites");
// Static
SUB("atom");
SUB("json");
SUB("favorites/json");

}
}
