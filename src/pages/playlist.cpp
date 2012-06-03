if((id = route("playlist", path, sub))){
    SUB("");
    SUB("remove");
    SUB("move");
    SUB("delete");
    SUB("edit");
    SUB("json");
}
PATH("playlist/new");
