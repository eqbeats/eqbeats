ROUTE("track"){

Track t(id);
if(t){

SUB(""){
    HTML(t.title);
    tpl = "track.tpl";
    t.fill(dict);
    t.player(dict, true);
    Audio(&t).fill(dict);
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
SUB("json");

SUB("original") { code = 0; o << Http::download(Audio(&t).original()); }
SUB("vorbis")   { code = 0; o << Http::download(Audio(&t).vorbis());   }
SUB("mp3")      { code = 0; o << Http::download(Audio(&t).mp3());      }

if(sub.substr(0,3) == "art"){
    Art art(id);
    if(art){
        SUB("art")        { code = 0; o << Http::download(art.full());      }
        SUB("art/medium") { code = 0; o << Http::download(art.medium());    }
        SUB("art/thumb")  { code = 0; o << Http::download(art.thumbnail()); }
    }
}

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
