PATH("/track/new");
PATH("/tracks") redir = "/";
PATH("/tracks/search");

PATH("/tracks/latest"){
    HTML("Latest tracks");
    tpl = "tracklist-page.tpl";
    dict->SetValue("TITLE", "Latest tracks");
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
    Tracks::featured(15).fill(dict, "TRACKLIST");
}

PATH("/tracks/search/json");
PATH("/tracks/search/exact/json");

PATH("/tracks/latest/json");
PATH("/tracks/random/json");
PATH("/tracks/featured/json");

PATH("/tracks/latest/atom"){
    mime = "application/atom+xml";
    tpl = "atom-feed.tpl";
    dict->SetValue("WHAT", "Latest tracks");
    dict->SetValue("FEED_LINK", "/tracks/latest/atom");
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
    dict->SetValue("FEED_LINK", "/tracks/featured/atom");
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

if(path.substr(0,12) == "/tracks/tag/"){
    //std::string tag = path.substr(12);
    //Html::tracksPage(tag, Track::byTag(tag));
}
