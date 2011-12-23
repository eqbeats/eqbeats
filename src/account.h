#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include "user.h"

class Account: public User{

    public:
        Account(){}
        Account(int nId);
        Account(const User &user);

        std::string email() const { return _email; }

        bool setPassword(const std::string &nPass);
        bool setName(const std::string &nName);
        bool setEmail(const std::string &nEmail);
        void commit();

        static bool exists(const std::string &sName, const std::string &sEmail=std::string());
        static Account create(const std::string &nName, const std::string &nPass, const std::string &nEmail);

        static bool validEmail(const std::string &nEmail);
        bool checkPassword(const std::string &pw);

    private:
        std::string getEmail();
        Account(int nId, const std::string &nName, const std::string &nEmail);
        std::string _email;

};

#endif // ACCOUNT_H
