PATH("track/new");
PATH("tracks"); //Http::redirect("/");
PATH("tracks/search");
PATH("tracks/latest");
PATH("tracks/random");
PATH("tracks/featured");
PATH("tracks/search/json");
PATH("tracks/search/exact/json");
PATH("tracks/latest/json");
PATH("tracks/random/json");
PATH("tracks/featured/json");
PATH("tracks/latest/atom");
PATH("tracks/featured/atom");

if(path.substr(0,12) == "/tracks/tag/"){
    //std::string tag = path.substr(12);
    //Html::tracksPage(tag, Track::byTag(tag));
}
