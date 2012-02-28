#include "repl.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>

Repl::Repl(const char *sock){
    s = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, sock);
    if(connect(s, (struct sockaddr *)&addr, SUN_LEN(&addr)) == -1){
        std::string err = (std::string) "couldn't connect to " + sock;
        perror(err.c_str());
        s = 0;
    }
}

Repl& Repl::operator=(const Repl &r){
    s = r.s;
    return *this;
}

std::string Repl::exec(std::string cmd){
    if(!s) return std::string();
    cmd += '\n';
    if(send(s, cmd.c_str(), cmd.size(), MSG_NOSIGNAL) == -1){
        perror("send error");
        s = 0;
        return std::string();
    }
    std::string line;
    char str[16];
    char *nl = 0;
    int t;
    while(!nl){
        t = recv(s, str, 16, MSG_NOSIGNAL);
        if(t <= 0){
            perror("recv error");
            s = 0;
            return std::string();
        }
        nl = strchr(str, '\n');
        if(nl) *nl = '\0';
        line.append(str, t-1);
    }
    return line;
}
