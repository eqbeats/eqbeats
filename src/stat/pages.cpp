#include "pages.h"
#include "stat.h"
#include <core/cgi.h>
#include <account/user.h>
#include <track/track.h>

#define addMeasurement(name) \
    dict = doc->dict()->AddSectionDictionary("MEASUREMENT");\
    dict->SetValue("MEASUREMENT", name);\
    dict->SetIntValue("VALUE", Stat::get(name, uid, tid, false));

#define addUniqueMeasurement(name) \
    dict = doc->dict()->AddSectionDictionary("UNIQUE_MEASUREMENT");\
    dict->SetValue("MEASUREMENT", name);\
    dict->SetIntValue("VALUE", Stat::get(name, uid, tid, true));

#define addDailyMeasurement(name) \
    dict = doc->dict()->AddSectionDictionary("DAILY_MEASUREMENT");\
    dict->SetValue("MEASUREMENT", name);\
    days = Stat::getDays(name, tid, false);\
    for(std::map<std::string, int>::iterator day = days.begin(); day != days.end(); day++){\
        inner = dict->AddSectionDictionary("DAY");\
        inner->SetValue("DAY", day->first);\
        inner->SetIntValue("VALUE", day->second);\
    }

#define addDailyUniqueMeasurement(name) \
    dict = doc->dict()->AddSectionDictionary("DAILY_UNIQUE_MEASUREMENT");\
    dict->SetValue("MEASUREMENT", name);\
    days = Stat::getDays(name, tid, true);\
    for(std::map<std::string, int>::iterator day = days.begin(); day != days.end(); day++){\
        inner = dict->AddSectionDictionary("DAY");\
        inner->SetValue("DAY", day->first);\
        inner->SetIntValue("VALUE", day->second);\
    }


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
            addUniqueMeasurement("trackView");
            addUniqueMeasurement("trackPlay");
            addUniqueMeasurement("trackDownload");
            doc->dict()->ShowSection("HAS_DAILY");
            addDailyUniqueMeasurement("trackView");
            addDailyUniqueMeasurement("trackPlay");
            addDailyUniqueMeasurement("trackDownload");
            addDailyMeasurement("trackView");
            addDailyMeasurement("trackPlay");
            addDailyMeasurement("trackDownload");
            doc->dict()->ShowSection("HAS_REFERRERS");
            referrers = Stat::getReferrers(tid);\
            for(std::map<std::string, int>::iterator referrer = referrers.begin(); referrer != referrers.end(); referrer++){\
                inner = doc->dict()->AddSectionDictionary("REFERRER");\
                inner->SetValue("REFERRER", referrer->first);\
                inner->SetIntValue("VALUE", referrer->second);\
            }
        }
        addMeasurement("trackView");
        addMeasurement("trackPlay");
        addMeasurement("trackDownload");
    }

}
