#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <signal.h>
#include <sysexits.h>
#include <errno.h>

void chld_handler(int s){
    printf("Child exited.\n");
    exit(1);
}

int main(int argc, char** argv){
    pid_t pid;
    int in[2], out[2];
    int s, request, bytes_read;
    int max_fd;
    int i;
    char buffer[BUFSIZ];
    fd_set sockets, ready;
    struct sigaction sa_chld, sa_pipe;
    if(argc != 3){
        printf("Usage: %s CMD SOCK\n", argv[0]);
        return EX_USAGE;
    }

    memset(&sa_chld, 0, sizeof(struct sigaction));
    memset(&sa_pipe, 0, sizeof(struct sigaction));
    sa_chld.sa_handler = &chld_handler;
    sa_chld.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa_chld, NULL);

    sa_pipe.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa_pipe, NULL);

    pipe(in);
    pipe(out);
    if(!fork()){
        dup2(out[0], 0);
        close(out[1]);
        close(in[0]);
        dup2(in[1], 1);
        execl("/bin/sh", "/bin/sh", "-c", argv[1]);
    }
    else {
        close(out[0]);
        close(in[1]);
        unlink(argv[2]);
        s = socket(AF_UNIX, SOCK_STREAM, 0);
        max_fd = s;
        struct sockaddr_un sa;
        sa.sun_family = AF_UNIX;
        strcpy(sa.sun_path, argv[2]);
        bind(s, (struct sockaddr *) &sa, SUN_LEN(&sa));
        listen(s, 20);
        FD_ZERO(&sockets);
        FD_SET(s, &sockets);
        for(;;){
            ready = sockets;
            select(max_fd+1, &ready, NULL, NULL, NULL);
            for(i = 0; i <= max_fd; i++){
                if(FD_ISSET(i, &ready)){
                    if(i == s){
                        request = accept(s, NULL, NULL);
                        if(max_fd < request) max_fd = request;
                        FD_SET(request, &sockets);
                    }
                    else{
                        buffer[0] = '\0';
                        while(strchr(buffer, '\n') == NULL){
                            bytes_read = read(i, buffer, BUFSIZ-1);
                            if(bytes_read <= 0)
                                goto close;
                            write(out[1], buffer, bytes_read);
                        }
                        buffer[0] = '\0';
                        while(strchr(buffer, '\n') == NULL){
                            bytes_read = read(in[0], buffer, BUFSIZ-1);
                            if(write(i, buffer, bytes_read) == EPIPE)
                                goto close;
                        }
                    }
                }
                continue;
                close:
                    FD_CLR(i, &sockets);
                    close(i);
            }
        }
    }
}
