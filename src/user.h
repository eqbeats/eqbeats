#ifndef USER_H
#define USER_H

#include <stdlib.h>
#include <string>
#include <vector>

class User{

    public:
        User() { _id = -1; }
        User(int nId);
        int id() const { return _id; }
        std::string name() const { return _name; }

        std::string pageUrl() const { return pageUrl(_id); }
        static std::string pageUrl(int id);

        operator bool() const { return _id > 0; }
        bool operator==(const User &u){ return _id == u._id; }

        static std::vector<User> list(unsigned int n, unsigned int begin=0);
        static std::vector<User> listArtists(unsigned int n, unsigned int begin=0);

    protected:
        User(int nId, const std::string &nName) { _id = nId; _name = nName; }
        std::string _name;

    private:
        static std::vector<User> listHelper(const char* format, unsigned int n, unsigned int begin=0);
        int _id;

};

#endif //USER_H
