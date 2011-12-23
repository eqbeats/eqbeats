#include "timer.h"
#include <sys/time.h>

struct timeval ref;

long int timeval_subtract(struct timeval *t2, struct timeval *t1){
    return (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
}

void resetTimer(){
    gettimeofday(&ref, 0);
}

long int usecs(){
    struct timeval end;
    gettimeofday(&end, 0);
    return timeval_subtract(&end, &ref);
}
