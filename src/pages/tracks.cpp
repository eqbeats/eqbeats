PATH("/track/new");
PATH("/tracks") redir = "/";

// Pages

PATH("/tracks/search"){
    std::string q = cgi("q");
    HTML(q);
    tpl = "tracklist-page.tpl";
    dict->SetValue("TITLE", q);
    TrackList::search(q).fill(dict, "TRACKLIST");
}

if(path.substr(0,12) == "/tracks/tag/"){
    std::string tag = path.substr(12);
    HTML(tag);
    tpl = "tracklist-page.tpl";
    dict->SetValue("TITLE", tag);
    TrackList::tag(tag).fill(dict, "TRACKLIST");
}

PATH("/tracks/latest"){
    HTML("Latest tracks");
    tpl = "tracklist-page.tpl";
    dict->SetValue("TITLE", "Latest tracks");
    dict->SetValueAndShowSection("FEED_URL", "/tracks/latest/atom", "HAS_FEED");
    int p = number(cgi("p"));
    if(p < 1) p = 1;
    TrackList l = Tracks::latest(16, 15*(p-1));
    if(p > 1)
        dict->SetValueAndShowSection("PREV", number(p-1), "HAS_PREV");
    if(l.size() == 16)
        dict->SetValueAndShowSection("NEXT", number(p+1), "HAS_NEXT");
    if(!l.empty()) l.pop_back();
    l.fill(dict, "TRACKLIST");
}
PATH("/tracks/random"){
    HTML("Random tracks");
    tpl = "tracklist-page.tpl";
    dict->SetValue("TITLE", "Random tracks");
    Tracks::random(15).fill(dict, "TRACKLIST");
}
PATH("/tracks/featured"){
    HTML("Featured tracks");
    tpl = "tracklist-page.tpl";
    dict->SetValue("TITLE", "Featured tracks");
    dict->SetValueAndShowSection("FEED_URL", "/tracks/featured/atom", "HAS_FEED");
    Tracks::featured(15).fill(dict, "TRACKLIST");
}

// JSON

TrackList json_tracks;
bool is_json_array = false;
PATH("/tracks/search/json")
    is_json_array = true, json_tracks = TrackList::search(cgi("q"));
PATH("/tracks/search/exact/json")
    is_json_array = true, json_tracks = TrackList::exactSearch(cgi("artist"), cgi("track"));
PATH("/tracks/latest/json")
    is_json_array = true, json_tracks = Tracks::latest(50);
PATH("/tracks/random/json")
    is_json_array = true, json_tracks = Tracks::random(50);
PATH("/tracks/featured/json")
    is_json_array = true, json_tracks = Tracks::featured(50);
if(is_json_array){
    JSON();
    tpl = "array-json.tpl";
    Dict *item, *data;
    for(TrackList::const_iterator i=json_tracks.begin(); i!=json_tracks.end(); i++){
        item = dict->AddSectionDictionary("ITEM");
        data = item->AddIncludeDictionary("DATA");
        data->SetFilename("track-json.tpl");
        i->fill(data);
    }
}

// Feeds

PATH("/tracks/latest/atom"){
    mime = "application/atom+xml";
    tpl = "atom-feed.tpl";
    dict->SetValue("WHAT", "Latest tracks");
    dict->SetValue("FEED_URL", "/tracks/latest/atom");
    TrackList l = Tracks::latest(50);
    if(l.empty()) dict->SetValue("UPDATED", "1970-01-01 01:00:00+00");
    else dict->SetValue("UPDATED", l[0].date);
    for(TrackList::const_iterator i=l.begin(); i!=l.end(); i++)
        i->fill(dict->AddSectionDictionary("TRACK"));
}
PATH("/tracks/featured/atom"){
    mime = "application/atom+xml";
    tpl = "atom-feed.tpl";
    dict->SetValue("WHAT", "Featured tracks");
    dict->SetValue("FEED_URL", "/tracks/featured/atom");
    TrackList l = Tracks::featured(50);
    // Last updated: "today at midnight"
    time_t raw = time(NULL);
    struct tm *now = gmtime(&raw);
    char now_str[128];
    strftime(now_str, 128, "%F 01:00:00+00", now);
    dict->SetValue("UPDATED", now_str);
    // Entries
    for(TrackList::const_iterator i=l.begin(); i!=l.end(); i++)
        i->fill(dict->AddSectionDictionary("TRACK"));
}
