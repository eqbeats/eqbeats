PATH(""){
    HTML("");
    tpl = "home.tpl";
    Tracks::latest(7).fill(dict, "LATEST_TRACKS");
    Tracks::featured(7).fill(dict, "FEATURED_TRACKS");
    Tracks::random(3).fill(dict, "RANDOM_TRACKS");
}
