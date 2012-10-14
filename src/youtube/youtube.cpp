#include "youtube.h"
#include "../misc/repl.h"
#include <core/cgi.h>
#include <account/session.h>
#include <core/db.h>
#include <text/text.h>
#include <track/extended.h>

#include <unistd.h>

Youtube::Youtube(int uid): _uid(uid) { }

Youtube::operator bool() const{
    DB::Result r = DB::query("SELECT 1 FROM youtube_refresh_tokens WHERE user_id = " + number(_uid) + " LIMIT 1");
    return r.size() > 0;
}

void Youtube::unlink() {
    DB::query("DELETE FROM youtube_refresh_tokens WHERE user_id = " + number(_uid));
}

bool Youtube::link(std::string & code){
    Repl ytmgr(("/tmp/ytmgr-" + number(getuid()) + ".sock").c_str());
    return ytmgr.exec("auth " + code + " " + number(_uid)).find("OK") != 0;
}

bool Youtube::upload(ExtendedTrack & t){
    Repl ytmgr(("/tmp/ytmgr-" + number(getuid()) + ".sock").c_str());
    return ytmgr.exec("upload " + number(t.id)).find("OK") != 0;
}
