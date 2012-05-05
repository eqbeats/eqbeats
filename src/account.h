#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "user.h"

class Account: public User{

    public:
        Account(){}
        Account(int nId);

        std::string email() const { return _email; }
        std::string about() const { return _about; }
        bool notify() const { return _notify; }
        std::string license() const { return _license; }

        bool setPassword(const std::string &nPass);
        bool setName(const std::string &nName);
        bool setEmail(const std::string &nEmail);
        void setAbout(const std::string &nAbout);
        void setNotify(bool n){ _notify = n; }
        void setLicense(const std::string &nLicense, bool def=true, bool retroactive=false);
        void commit();

        static bool exists(const std::string &sName, const std::string &sEmail=std::string());
        static Account create(const std::string &nName, const std::string &nPass, const std::string &nEmail);

        static bool validEmail(const std::string &nEmail);
        bool checkPassword(const std::string &pw);

        static std::vector<Account> select(const std::string &p, const std::string &param=std::string());
        static std::vector<Account> listArtists(unsigned int n, unsigned int begin=0);
        static std::vector<Account> search(const std::string &q);

    private:
        Account(int nId, const std::string &nName, const std::string &nEmail, const std::string &nAbout, bool nNotify);
        std::string _email;
        std::string _about;
        bool _notify;
        std::string _license;

};

#endif // ACCOUNT_H
