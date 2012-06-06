ROUTE("track"){

Track t(id);
if(t){

SUB(""){
    HTML(t.title);
    tpl = "track.tpl";
    t.fill(dict);
}

SUB("delete");
SUB("rename");
SUB("notes");
SUB("upload");
SUB("art/upload");
SUB("publish");
SUB("comment");
SUB("favorite");
SUB("unfavorite");
SUB("report");
SUB("flags");
SUB("tags");
SUB("license");
SUB("youtube_upload");
SUB("playlist");

// Static
SUB("original");
SUB("vorbis");
SUB("mp3");

SUB("art");
SUB("art/medium");
SUB("art/thumb"){
    Art art(id);
    if(art){
        o << Http::download(art.thumbnail());
        code = 0;
    }
}

SUB("json");

}

SUB("embed"){
    if(t){
        t.fill(dict);
        t.player(dict, true);
    }
    dict->ShowSection(t ? "FOUND" : "NOT_FOUND");
    mime = "text/html";
    tpl = "player-embed.tpl";
}

}
