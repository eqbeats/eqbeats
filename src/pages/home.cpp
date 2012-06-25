PATH(""){
    HTML("");
    tpl = "html/home.tpl";
    rootDict->SetValueAndShowSection("FEED_URL", "/tracks/latest/atom", "FEED");
    Tracks::latest(7).fill(dict, "LATEST_TRACKS");
    Tracks::featured(7).fill(dict, "FEATURED_TRACKS");
    Tracks::random(3).fill(dict, "RANDOM_TRACKS");
}
