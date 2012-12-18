/*
 * The server sends a space-separated datagram that
 * looks like "TYPE TID UID ADDR", i.e. "trackPlay 456 123 127.0.0.1",
 * "userVisit 0 123 127.0.0.1", etc.
 * From there on, we dispatch that info to the appropriate track / user
 * stat files, as a JSON object.
 * { 'type':'trackPlay', 'id':123, 'addr':'salted md5 shit for anonymity',
 *   'timestamp':1234567890, 'unique':-1 }
 * The unique value will be filled later, per day, and addr will be pruned.
 */

#include <errno.h>
#include <fcntl.h>
#include <iterator>
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include <text/text.h>

#include <iostream>
#include <string.h>

char nibble2Hex(int in){
    switch(in){
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'a';
        case 11: return 'b';
        case 12: return 'c';
        case 13: return 'd';
        case 14: return 'e';
        default: return 'f';
    }
}
std::string trimMD5Hex(std::string in){
    unsigned char md[16];
    MD5((const unsigned char*)in.c_str(), in.size(), md);
    std::string out;
    for(int i=0; i<5; i++){
        out += nibble2Hex((md[i] & 240) >> 4); /* 240 = 0b11110000 */
        out += nibble2Hex(md[i] & 15);         /* 15  = 0b00001111 */
    }
    return out;
}

std::string salt_;

std::string salt(){
    if(!salt_.empty())
        return salt_;
    int f = open(((std::string)getenv("EQBEATS_DIR") + "/.udpstat.salt").c_str(), O_RDONLY);
    if(f > -1){
        char buffer[10];
        read(f, buffer, 10);
        close(f);
        salt_ = std::string(buffer, 10);
        return salt_;
    }
    else {
        int urandom = open("/dev/urandom", O_RDONLY);
        char buffer[10];
        read(urandom, buffer, 10);
        close(urandom);
        salt_ = trimMD5Hex(std::string(buffer, 10));

        umask(0777 % 0640);
        FILE* f_ = fopen(((std::string)getenv("EQBEATS_DIR") + "/.udpstat.salt").c_str(), "w");
        fprintf(f_, "%s", salt_.c_str());
        fclose(f_);

        return salt_;
    }
}

void append(std::string filepath, std::string in){
    int f = open(filepath.c_str(), O_WRONLY | O_CREAT, 0644);
    if(lseek(f, 0, SEEK_END) == 0)
        write(f, "[\n", 2);
    else {
        lseek(f, -3, SEEK_END);
        write(f, ",\n", 2);
    }
    write(f, in.c_str(), in.size());
    write(f, "\n]\n", 3);
    close(f);
}

int main(int argc, char** argv){
    int s;
    char buffer[300];
    size_t len;
    std::string sockloc = (std::string)getenv("EQBEATS_DIR") + "/udpstat.sock";

    unlink(sockloc.c_str());
    s = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un sa;
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, sockloc.c_str());
    bind(s, (struct sockaddr *) &sa, sockloc.size() + 2);
    while(true){
        len = recvfrom(s, buffer, 300, 0, NULL, NULL);
        if(len <= 0)
            continue;
        std::string contents = std::string(buffer, len);
        std::string type;
        std::string::iterator i;
        for(i = contents.begin(); !(*i == ' '); i++){
            type += *i;
        }

        i++;

        std::string buf;
        for(; !(*i == ' '); i++){
            buf += *i;
        }
        int tid = number(buf);

        i++;
        buf = "";

        for(; !(*i == ' '); i++){
            buf += *i;
        }
        int uid = number(buf);

        i++;

        std::string addr;
        for(; i != contents.end() && !(*i == ' ') && !(*i == '\n'); i++){
            addr += *i;
        }
        addr = trimMD5Hex(addr + salt());


        std::string entry = "{ 'type':'" + type + "', "+
                            (tid > 0?"'tid':" + number(tid) + ", ":"")+
                            (type == "userView"?"'uid':" + number(uid) + ", ":"")+
                            "'addr':'" + addr + "', "+
                            "'timestamp':" + number(time(NULL)) + ", "+
                            "'unique':-1 }";


        struct stat st;
        if(stat(((std::string)getenv("EQBEATS_DIR") + "/udpstat/").c_str(), &st) == -1 && errno == ENOENT){
            mkdir(((std::string)getenv("EQBEATS_DIR") + "/udpstat/").c_str(), 0755);
            mkdir(((std::string)getenv("EQBEATS_DIR") + "/udpstat/users/").c_str(), 0755);
            mkdir(((std::string)getenv("EQBEATS_DIR") + "/udpstat/tracks/").c_str(), 0755);
        }

        if(tid > 0){
            append((std::string)getenv("EQBEATS_DIR") + "/udpstat/tracks/" + number(tid) + ".json", entry);
        }
        if(uid > 0){
            append((std::string)getenv("EQBEATS_DIR") + "/udpstat/users/" + number(uid) + ".json", entry);
        }
    }
    return 0; // lol
}
