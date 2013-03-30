#ifndef SOCIAL_FOLLOWER_H
#define SOCIAL_FOLLOWER_H

#include <account/list.h>
#include <track/list.h>

class Follower{

    public:

        Follower(int uid): id(uid) {}

        bool favorited(int tid);
        TrackList favorites();

        bool following(int uid);
        AccountList followed();
        AccountList followers();

    private:
        int id;

};


#endif // SOCIAL_FOLLOWER_H
