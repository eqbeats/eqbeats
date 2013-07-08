#include "youtube.h"
#include "../misc/repl.h"
#include <core/cgi.h>
#include <account/session.h>
#include <core/db.h>
#include <text/text.h>
#include <track/extended.h>

#include <unistd.h>
#include <sys/wait.h>

Youtube::Youtube(int uid): _uid(uid) { }

Youtube::operator bool() const{
    DB::Result r = DB::query("SELECT 1 FROM youtube_refresh_tokens WHERE user_id = " + number(_uid) + " LIMIT 1");
    return r.size() > 0;
}

void Youtube::unlink() {
    DB::query("DELETE FROM youtube_refresh_tokens WHERE user_id = " + number(_uid));
}

std::string ytmgrPath(){
    return eqbeatsDir() + "/tools/ytmgr.py";
}

bool Youtube::link(std::string & code){
    int rc;
    pid_t pid = fork();
    if(pid == 0){
        if(execl(ytmgrPath().c_str(), "ytmgr", "auth", code.c_str(), number(_uid).c_str(), NULL) == -1)
            exit(1);
    }
    waitpid(pid, &rc, 0);
    return WIFEXITED(rc);
}

bool Youtube::upload(ExtendedTrack & t){
    if(fork() == 0)
        execl(ytmgrPath().c_str(), "ytmgr", "upload", number(t.id).c_str(), NULL);
    return true;
}
