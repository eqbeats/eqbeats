#include "hash.h"

#include <nettle/md5.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long int ulong;

const char *hex = "0123456789abcdef";

std::string md5(const std::string &str){
    struct md5_ctx ctx;
    md5_init(&ctx);
    md5_update(&ctx, str.size(), (uint8_t*) str.c_str());
    uint8_t digest[MD5_DIGEST_SIZE];
    md5_digest(&ctx, MD5_DIGEST_SIZE, digest);
    std::string buf;
    for(int i=0; i<MD5_DIGEST_SIZE; i++)
        (buf += hex[(digest[i]>>4) & 0x0F]) += hex[digest[i] & 0x0F];
    return buf;
}

std::string pool = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";

std::string randomString(size_t length){
    std::string buf;
    while(buf.size() < length)
        buf += pool[rand() % pool.size()];
    return buf;
}
