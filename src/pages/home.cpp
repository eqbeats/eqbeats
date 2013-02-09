#include "pages.h"
#include <core/cgi.h>
#include <core/db.h>
#include <track/list.h>

void Pages::home(Document *doc){

    if(path != "") return;

    doc->setHtml("html/home.tpl");
    doc->rootDict()->SetValueAndShowSection("FEED_URL", "/tracks/latest/atom", "FEED");

    DB::Result news = DB::query("SELECT title, url FROM ticker WHERE 'now' < expire ORDER BY expire DESC");
    if(news.empty()) news = DB::query("SELECT title, url FROM ticker ORDER BY expire DESC LIMIT 1");
    if(!news.empty()){
        Dict *latest = doc->dict()->AddSectionDictionary("LATEST_NEWS");
        latest->SetValue("NEWS_TITLE", news[0][0]);
        latest->SetValue("NEWS_URL", news[0][1]);
        if(news.size() > 1){
            Dict *ticker = doc->dict()->AddSectionDictionary("DYNAMIC_TICKER");
            for(DB::Result::const_iterator i=news.begin(); i!=news.end(); i++){
                Dict *item = ticker->AddSectionDictionary("NEWS");
                item->SetValue("NEWS_TITLE", i->at(0));
                item->SetValue("NEWS_URL", i->at(1));
            }
        }
    }

    Tracks::featured(15).fill(doc->dict(), "FEATURED_TRACKS");
    Tracks::latest(5).fill(doc->dict(), "LATEST_TRACKS");
    Tracks::random(5).fill(doc->dict(), "RANDOM_TRACKS");

}
