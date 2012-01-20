#ifndef USER_H
#define USER_H

#include <stdlib.h>
#include "db.h"
#include "track.h"

class User{

    public:
        User() { _id = 0; }
        User(int nId);
        int id() const { return _id; }
        std::string name() const { return _name; }

        std::string url() const { return url(_id); }
        static std::string url(int id);

        operator bool() const { return _id > 0; }
        bool operator==(const User &u){ return _id == u._id; }

        static std::vector<User> list(unsigned int n, unsigned int begin=0);
        static std::vector<User> listArtists(unsigned int n, unsigned int begin=0);
        static std::vector<User> search(const std::string &q);

        std::vector<std::string> followers() const; // email addresses
        std::vector<User> following() const;
        void follow(int uid);
        void unfollow(int uid);
        bool isFollowing(int uid) const;
        int followersCount() const;

        bool isFavorite(int tid) const;
        void addToFavorites(int tid);
        void removeFromFavorites(int tid);

    protected:
        User(int nId, const std::string &nName) { _id = nId; _name = nName; }
        std::string _name;
        int _id;

    private:
        static std::vector<User> listHelper(bool artists, unsigned int n, unsigned int begin=0);
        static std::vector<User> resultToVector(const DB::Result &r);

};

#endif //USER_H
