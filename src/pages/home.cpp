PATH(""){
    HTML("");
    tpl = "home.tpl";
    TrackList latest("SELECT %s FROM users, tracks WHERE %s AND visible='t' ORDER BY tracks.date DESC");
    latest.fill(dict, "LATEST_TRACKS");
}
