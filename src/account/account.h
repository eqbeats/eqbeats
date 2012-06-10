#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "user.h"

class Account: public User {

    public:

        Account(int nId);

        std::string email;
        std::string about;
        bool notify;
        std::string license;

        void fill(Dict*) const;

};

#endif // ACCOUNT_H
