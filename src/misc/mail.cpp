#include "mail.h"
#include <core/path.h>
#include <text/text.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <log/log.h>

void sendMail(const char *to, const char *data){
    int fd[2];
    pipe(fd);
    pid_t child = fork();
    if(child == 0){
        close(fd[1]);
        close(0);
        dup(fd[0]);
        freopen("/dev/null","a",stdout);
        freopen("/dev/null","a",stderr);
        execlp("sendmail", "sendmail", to, NULL);
        log("sendmail error: %m");
        exit(1);
    }
    else if(child > 0){
        close(fd[0]);
        time_t now = time(NULL);
        struct tm *utc = gmtime(&now);
        char buf[50];
        strftime(buf, 50, "%d %b %Y %H:%M %z", utc);
        std::string cnt = (std::string)
            "Date: " + buf + "\n"
            "To: <" + to + ">\n"
            "Reply-To: EqBeats <eqbeats@eqbeats.org>\n"
            "Content-Type: text/plain\n"
            + data;
        write(fd[1], cnt.c_str(), cnt.size());
        close(fd[1]);
        waitpid(child, NULL, 0);
    }
}
