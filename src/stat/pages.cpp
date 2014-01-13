#include "pages.h"
#include "stat.h"
#include <core/cgi.h>
#include <account/user.h>
#include <track/track.h>

void Pages::stats(Document *doc){

    std::string sub, sub2;
    int tid = route("track", path, sub);
    int uid = route("user", path, sub2);
    Track t(tid);
    User u;
    if(t) u = t.artist;
    else u = User(uid);

    if((t && sub == "stats") || (u && sub2 == "stats")){
        doc->setTemplate("json/stats.tpl", "application/json");

        Dict *dict, *inner, *innerer;
        map<std::string, int> days, referrers;
        if(t){
            doc->dict()->ShowSection("HAS_UNIQUE");
            Stat::fillMeasurement(doc->dict(), "trackView", uid, tid, true, false);
            Stat::fillMeasurement(doc->dict(), "trackPlay", uid, tid, true, false);
            Stat::fillMeasurement(doc->dict(), "trackDownload", uid, tid, true, false);
            doc->dict()->ShowSection("HAS_DAILY");
            Stat::fillMeasurement(doc->dict(), "trackView", uid, tid, true, true);
            Stat::fillMeasurement(doc->dict(), "trackPlay", uid, tid, true, true);
            Stat::fillMeasurement(doc->dict(), "trackDownload", uid, tid, true, true);
            Stat::fillMeasurement(doc->dict(), "trackView", uid, tid, false, true);
            Stat::fillMeasurement(doc->dict(), "trackPlay", uid, tid, false, true);
            Stat::fillMeasurement(doc->dict(), "trackDownload", uid, tid, false, true);
            doc->dict()->ShowSection("HAS_REFERRERS");
            referrers = Stat::getReferrers(tid);
            for(std::map<std::string, int>::iterator referrer = referrers.begin(); referrer != referrers.end(); referrer++){
                inner = doc->dict()->AddSectionDictionary("REFERRER");
                inner->SetValue("REFERRER", referrer->first);
                inner->SetIntValue("VALUE", referrer->second);
            }
        }
        Stat::fillMeasurement(doc->dict(), "trackView", uid, tid, false, false);
        Stat::fillMeasurement(doc->dict(), "trackPlay", uid, tid, false, false);
        Stat::fillMeasurement(doc->dict(), "trackDownload", uid, tid, false, false);
    }

}
