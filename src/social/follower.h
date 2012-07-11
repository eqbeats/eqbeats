#ifndef SOCIAL_FOLLOWER_H
#define SOCIAL_FOLLOWER_H

#include <account/list.h>
#include <track/list.h>

class Follower{

    public:

        Follower(User *u): user(u) {}

        bool favorited(int tid);
        TrackList favorites();

        bool following(int uid);
        AccountList followed();
        std::vector<std::string> followers(); // email addresses

    private:
        User *user;

};


#endif // SOCIAL_FOLLOWER_H
