if((id = route("user", path, sub))){
    SUB("");
    SUB("comment");
    SUB("follow");
    SUB("unfollow");
    SUB("favorites");
    // Static
    SUB("atom");
    SUB("json");
    SUB("favorites/json");
}
