#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "user.h"

class Account: public User{

    public:
        Account(){}
        Account(int nId);
        //Account(const User &user);

        std::string email() const { return _email; }
        std::string about() const { return _about; }
        bool notify() const { return _notify; }

        bool setPassword(const std::string &nPass);
        bool setName(const std::string &nName);
        bool setEmail(const std::string &nEmail);
        void setAbout(const std::string &nAbout);
        void setNotify(bool n){ _notify = n; }
        void commit();

        static bool exists(const std::string &sName, const std::string &sEmail=std::string());
        static Account create(const std::string &nName, const std::string &nPass, const std::string &nEmail);

        static bool validEmail(const std::string &nEmail);
        bool checkPassword(const std::string &pw);

    private:
        Account(int nId, const std::string &nName, const std::string &nEmail, bool nNotify);
        std::string _email;
        std::string _about;
        bool _notify;

};

#endif // ACCOUNT_H
