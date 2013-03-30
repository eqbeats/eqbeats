#ifndef ACCOUNT_LIST_H
#define ACCOUNT_LIST_H

#include "account.h"
#include <core/db.h>

#include <vector>

class AccountList : public std::vector<Account> {

    public:

        AccountList(){}
        AccountList(const std::string &query_fmt);

        static AccountList all(int n, int offset=0);
        static AccountList artists(int n, int offset=0);
        static AccountList search(const std::string &q);

        Dict* fill(Dict*, const std::string &marker);
        void fillJson(Dict*);

    private:
        void extract(const DB::Result &r);

};

#endif // ACCOUNT_LIST_H
