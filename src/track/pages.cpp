#include "pages.h"
#include "audio.h"
#include "extended.h"
#include <core/cgi.h>
#include <core/db.h>
#include <media/art.h>
#include <session/session.h>
#include <render/document.h>
#include <text/text.h>

void trackPages(Document *doc){
    std::string sub;
    int tid = route("track", path, sub);
    ExtendedTrack t(tid);
    bool actor = cgi.getEnvironment().getRequestMethod() == "POST" && t.artist == Session::user();

    if(sub == "embed"){
        if(t){
            t.fill(doc->dict());
            t.player(doc->dict(), true);
        }
        doc->setTemplate("html/player-embed.tpl", "text/html", t ? 200 : 404);
        doc->dict()->ShowSection(t ? "FOUND" : "NOT_FOUND");
        return;
    }

    else if(!t)
        return;

    // Access

    if(sub == ""){
        doc->setHtml("html/track.tpl", t.title);
        doc->rootDict()->SetValueAndShowSection("TID", number(t.id), "HAS_OEMBED");
        t.fill(doc->dict());
        t.player(doc->dict(), true);
        Audio(&t).fill(doc->dict());
        Dict *embed = doc->dict()->AddIncludeDictionary("EMBED_CODE");
        embed->SetFilename("embed-code.tpl");
        embed->SetIntValue("WIDTH", 150);
        t.Track::fill(embed);
    }

    else if(sub == "json"){
        doc->setJson("json/track.tpl");
        t.fill(doc->dict());
    }

    // Actions

    else if(sub == "rename"){
        if(actor){
            std::string title = cgi("title");
            if(!title.empty() && title != t.title){
                DB::query("UPDATE tracks SET title = $1 WHERE id = " + number(t.id), title);
                t.title = title;
                Audio(&t).updateTags();
            }
        }
        doc->redirect(t.url());
    }

    else if(sub == "tags"){
        if(actor)
            DB::query("UPDATE tracks SET tags = regexp_split_to_array(lower($1), E' *, *') WHERE id = " + number(t.id), cgi("tags"));
        doc->redirect(t.url());
    }

    else if(sub == "notes"){
        if(actor)
            DB::query("UPDATE tracks SET notes = $1 WHERE id = " + number(t.id), cgi("notes"));
        doc->redirect(t.url());
    }

    else if(sub == "flags"){
        if(actor)
            DB::query("UPDATE tracks SET airable = $1 WHERE id = " + number(t.id), cgi.queryCheckbox("airable") ? "t" : "f");
        doc->redirect(t.url());
    }

    // Download

    else if(sub == "original") doc->download(Audio(&t).original());
    else if(sub == "vorbis")   doc->download(Audio(&t).vorbis());
    else if(sub == "mp3")      doc->download(Audio(&t).mp3());

    else if(sub.substr(0,3) == "art"){
        std::string base = t.artist.name + " - " + t.title;
        Art art(tid);
        if(art){
                 if(sub == "art")        doc->download(art.full().setBaseName(base));
            else if(sub == "art/medium") doc->download(art.medium().setBaseName(base + ".medium"));
            else if(sub == "art/thumb")  doc->download(art.thumbnail().setBaseName(base + ".thumb"));
        }
    }

}
