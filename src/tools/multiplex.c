#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

int main(int argc, char** argv){
    pid_t pid;
    int in[2], out[2], s, request, bytes_read;
    char buffer[BUFSIZ];
    if(argc != 3){
        printf("Usage: %s CMD SOCK\n", argv[0]);
        return 1;
    }
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
        struct sockaddr_un sa;
        sa.sun_family = AF_UNIX;
        strcpy(sa.sun_path, argv[2]);
        bind(s, (struct sockaddr *) &sa, SUN_LEN(&sa));
        listen(s, 20);
        for(;;){
            request = accept(s, NULL, NULL);
            buffer[0] = '\0';
            while(strchr(buffer, '\n') == NULL){
                bytes_read = read(request, buffer, BUFSIZ-1);
                if(bytes_read <= 0) goto close;
                write(out[1], buffer, bytes_read);
            }
            buffer[0] = '\0';
            while(strchr(buffer, '\n') == NULL){
                bytes_read = read(in[0], buffer, BUFSIZ-1);
                write(request, buffer, bytes_read);
            }
            close:
            close(request);
        }
    }
}
