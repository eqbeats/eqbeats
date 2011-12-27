#include <pcrecpp.h>
#include <stdio.h>
#include "account.h"
#include "utils.h"

Account::Account(int nId){
    _id = 0;
    if(nId<=0) return;
    DB::Result r = DB::query("SELECT name, email, about FROM Users WHERE id=" + number(nId));
    if(!r.empty()){
        _id = nId;
        _name = r[0][0];
        _email = r[0][1];
        _about = r[0][2];
    }
}

//Account::Account(const User &user): Account(user.id()){}

Account::Account(int nId, const std::string &nName, const std::string &nEmail)
    : User(nId, nName){
    _email = nEmail;
}

bool Account::setPassword(const std::string &nPass){
    if(nPass.empty() || nPass.size()>72) return false;
    DB::query("UPDATE users SET password = crypt($2, gen_salt('bf')) WHERE id = $1", number(_id), nPass);
    return true;
}

bool Account::setName(const std::string &nName){
    if(nName == _name) return true;
    if(nName.empty() || nName.size()>200) return false;
    if(exists(nName)) return false;
    _name = nName;
    return true;
}

void Account::setAbout(const std::string &nAbout){
    if(_about == nAbout) return;
    if(nAbout.size()>100000) return; // 100k
    _about = nAbout;
}

bool Account::setEmail(const std::string &nEmail){
    if(nEmail == _email) return true;
    //if(!validEmail(nEmail)) return false;
    if(exists(std::string(), nEmail)) return false;
    _email = nEmail;
    return true;
}

void Account::commit(){
    DB::query("UPDATE users SET name = $2, email = lower($3), about = $4 WHERE id = $1",
              number(_id), _name, _email, _about);
    DB::query("UPDATE comments SET author_name = $1 WHERE author_id = " + number(_id), _name);
}

bool Account::exists(const std::string &sName, const std::string &sEmail){
    return DB::query("SELECT EXISTS ( SELECT id FROM users WHERE name = $1 OR email = lower($2) )",
                        sName, sEmail)[0][0] == "t";
}

Account Account::create(const std::string &nName, const std::string &nPass, const std::string &nEmail){
    if(exists(nName, nEmail))
        return Account();
    DB::Result r = DB::query(
        "INSERT INTO users (name, password, email, registration, last_login) "
        "VALUES ($1, crypt($2, gen_salt('bf')), lower($3), 'now', 'now') "
        "RETURNING id", nName, nPass, nEmail);
    return r.empty() ? Account() : Account(number(r[0][0]), nName, nEmail);
}

bool Account::validEmail(const std::string &nEmail){
    return pcrecpp::RE("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?", pcrecpp::RE_Options().set_caseless(true)).FullMatch(nEmail);
}

bool Account::checkPassword(const std::string &pw){
    return DB::query("SELECT EXISTS (SELECT 1 FROM users WHERE id = " + number(_id) +
                     " AND password = crypt($1, password))", pw)[0][0] == "t";
}
