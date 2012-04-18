#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class Track;

class User{

    public:
        User() { _id = 0; }
        User(int nId);
        User(const std::string &nName);
        User(int nId, const std::string &nName) { _id = nId; _name = nName; }
        int id() const { return _id; }
        std::string name() const { return _name; }

        std::string url() const { return url(_id); }
        static std::string url(int id);

        bool hasYoutube();

        operator bool() const { return _id > 0; }
        bool operator==(const User &u) const { return _id == u._id; }
        bool operator!=(const User &u) const { return _id != u._id; }

        static std::vector<User> select(const std::string &p, const std::string &param=std::string());
        static std::vector<User> list(unsigned int n, unsigned int begin=0);
        static std::vector<User> listArtists(unsigned int n, unsigned int begin=0);
        static std::vector<User> search(const std::string &q);
        static User withEmail(const std::string &email);

        std::vector<Track> tracks(bool all=false);

    protected:
        std::string _name;
        int _id;

};

#endif //USER_H
