#ifndef USER_H
#define USER_H

#include <render/template.h>

class User{

    public:

        User() { id = 0; }
        User(int uid);
        User(const std::string &email);
        User(int uid, const std::string &nName) { id = uid; name = nName; }

        int id;
        std::string name;

        std::string url() const;

        void fill(Dict*) const;

        operator bool() const { return id > 0; }
        bool operator==(const User &u) const { return id == u.id; }

};

#endif //USER_H
