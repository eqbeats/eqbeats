#include "youtube.h"
#include "../misc/repl.h"
#include <core/cgi.h>
#include <account/session.h>
#include <core/db.h>
#include <text/text.h>
#include <track/extended.h>

#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>

Youtube::Youtube(int uid): _uid(uid) { }

Youtube::operator bool() const{
    DB::Result r = DB::query("SELECT 1 FROM youtube_refresh_tokens WHERE user_id = " + number(_uid) + " LIMIT 1");
    return r.size() > 0;
}

void Youtube::unlink() {
    DB::query("DELETE FROM youtube_refresh_tokens WHERE user_id = " + number(_uid));
}

#define YTMGR_PATH LIBEXEC_DIR"/ytmgr.py"

bool Youtube::link(std::string & code){
    int rc;
    pid_t pid = fork();
    if(pid == 0){
        if(execl(YTMGR_PATH, "ytmgr", "auth", code.c_str(), number(_uid).c_str(), NULL) == -1)
            exit(1);
    }
    waitpid(pid, &rc, 0);
    if(rc)
        syslog(LOG_ERR, "Failed to link YouTube account for user %d.", _uid);
    else
        syslog(LOG_NOTICE, "YouTube account linked successfully for user %d.", _uid);
    return WIFEXITED(rc);
}

bool Youtube::upload(ExtendedTrack & t){
    pid_t worker;
    if(!(worker = fork())){
        execl(YTMGR_PATH, "ytmgr", "upload", number(t.id).c_str(), NULL);
        exit(127);
    }
    else
        syslog(LOG_NOTICE, "Spawned YouTube uploader %d for track %d.", worker, t.id);
    return true;
}
