PATH("/oembed"){
    std::string url = cgi("url");
    drop("http://", url);
    drop("www.", url);
    if(drop("eqbeats.org", url)){
        std::string s;
        int tid = route("track", url, s);
        if(s == ""){
            Track t(tid);
            if(t){
                if(cgi("format") == "xml"){
                    mime = "text/xml";
                    tpl = "oembed/xml.tpl";
                }
                else{
                    JSON();
                    tpl = "oembed/json.tpl";
                }
                t.fill(dict);
                int width = number(cgi("maxwidth"));
                if(width <= 0) width = 150;
                dict->SetIntValue("WIDTH", width);
                Dict *embed = dict->AddIncludeDictionary("EMBED_CODE");
                embed->SetFilename("html/embed-code.tpl");
                embed->SetIntValue("WIDTH", width);
                t.Track::fill(embed);
            }
        }
    }
}
