ROUTE("track"){

ExtendedTrack t(id);
if(t){

// Access

SUB(""){
    HTML(t.title);
    tpl = "html/track.tpl";
    rootDict->SetValueAndShowSection("TID", number(t.id), "HAS_OEMBED");
    t.fill(dict);
    t.player(dict, true);
    Audio(&t).fill(dict);
    Dict *embed = dict->AddIncludeDictionary("EMBED_CODE");
    embed->SetFilename("embed-code.tpl");
    embed->SetIntValue("WIDTH", 150);
    t.Track::fill(embed);
}

SUB("json"){
    JSON();
    tpl = "json/track.tpl";
    t.fill(dict);
}

// Actions

SUB("rename"){
    if(t.artist == Session::user() && post){
        std::string title = cgi("title");
        if(!title.empty() && title != t.title){
            DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(t.id), title);
            t.title = title;
            Audio(&t).updateTags();
        }
    }
    redir = t.url();
}

SUB("tags"){
    if(t.artist == Session::user() && post)
        DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(t.id), cgi("tags"));
    redir = t.url();
}

SUB("notes"){
    if(t.artist == Session::user() && post)
        DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(t.id), cgi("notes"));
    redir = t.url();
}

SUB("flags"){
    if(t.artist == Session::user() && post)
        DB::query("UPDATE tracks SET airable = $1 WHERE id = " + number(t.id), cgi.queryCheckbox("airable") ? "t" : "f");
    redir = t.url();
}

// To do

SUB("delete");
SUB("upload");
SUB("art/upload");
SUB("publish");
SUB("comment");
SUB("favorite");
SUB("unfavorite");
SUB("report");
SUB("license");
SUB("youtube_upload");
SUB("playlist");

// Download

SUB("original") { code = 0; o << Http::download(Audio(&t).original()); }
SUB("vorbis")   { code = 0; o << Http::download(Audio(&t).vorbis());   }
SUB("mp3")      { code = 0; o << Http::download(Audio(&t).mp3());      }

if(sub.substr(0,3) == "art"){
    std::string base = t.artist.name + " - " + t.title;
    Art art(id);
    if(art){
        SUB("art")        { code = 0; o << Http::download(art.full().setBaseName(base)); }
        SUB("art/medium") { code = 0; o << Http::download(art.medium().setBaseName(base + ".medium")); }
        SUB("art/thumb")  { code = 0; o << Http::download(art.thumbnail().setBaseName(base + ".thumb")); }
    }
}

}

// Embed (even if the track wasn't found)

SUB("embed"){
    if(t){
        t.fill(dict);
        t.player(dict, true);
    }
    dict->ShowSection(t ? "FOUND" : "NOT_FOUND");
    mime = "text/html";
    tpl = "html/player-embed.tpl";
}

}
