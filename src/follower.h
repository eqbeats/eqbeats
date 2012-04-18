#ifndef FOLLOWER_H
#define FOLLOWER_H

#include "user.h"

class Follower: public User{

    public:
        Follower(const User &u): User(u) {}

        bool isFavorite(int tid) const;
        void addToFavorites(int tid);
        void removeFromFavorites(int tid);

        std::vector<std::string> followers() const; // email addresses
        std::vector<User> following() const;
        void follow(int uid);
        void unfollow(int uid);
        bool isFollowing(int uid) const;
        int followersCount() const;

};


#endif // FOLLOWER_H
