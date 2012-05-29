#include <pcrecpp.h>
#include <stdio.h>
#include "account.h"
#include "number.h"
#include "db.h"

Account::Account(int nId){
    _id = 0;
    if(nId<=0) return;
    DB::Result r = DB::query("SELECT name, email, about, notify, license FROM users WHERE id=" + number(nId));
    if(!r.empty()){
        _id = nId;
        _name = r[0][0];
        _email = r[0][1];
        _about = r[0][2];
        _notify = r[0][3][0] == 't';
        _license = r[0][4];
    }
}

Account::Account(int nId, const std::string &nName, const std::string &nEmail, const std::string &nAbout, bool nNotify)
    : User(nId, nName){
    _email = nEmail;
    _about = nAbout;
    _notify = nNotify;
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

void Account::setLicense(const std::string &nLicense, bool def, bool retroactive){
    if(_id <= 0) return;
    if(_license != nLicense && def){
        DB::query("UPDATE users SET license = $1 WHERE id = " + number(_id), nLicense);
        _license = nLicense;
    }
    if(retroactive)
        DB::query("UPDATE tracks SET license = $1 WHERE user_id = " + number(_id), nLicense);
}

std::string strToLower(std::string str){
    for(std::string::iterator i=str.begin(); i!=str.end(); i++)
        *i = std::tolower(*i);
    return str;
}

bool Account::setEmail(const std::string &nEmail){
    if(strToLower(nEmail) == strToLower(_email)){
        _email = nEmail;
        return true;
    }
    if(exists(std::string(), nEmail)) return false;
    _email = nEmail;
    return true;
}

void Account::commit(){
    if(_id <= 0) return;
    DB::query("UPDATE users SET name = $2, email = $3, about = $4, notify = $5 WHERE id = $1",
              number(_id), _name, _email, _about, _notify?"t":"f");
    DB::query("UPDATE comments SET author_name = $1 WHERE author_id = " + number(_id), _name);
}

bool Account::exists(const std::string &sName, const std::string &sEmail){
    return DB::query("SELECT EXISTS ( SELECT id FROM users WHERE lower(name) = lower($1) OR lower(email) = lower($2) )",
                        sName, sEmail)[0][0] == "t";
}

Account Account::create(const std::string &nName, const std::string &nPass, const std::string &nEmail){
    if(nName.empty() || nPass.empty() || nEmail.empty()) return Account();
    if(exists(nName, nEmail))
        return Account();
    DB::Result r = DB::query(
        "INSERT INTO users (name, password, email, registration, last_login) "
        "VALUES ($1, crypt($2, gen_salt('bf')), $3, 'now', 'now') "
        "RETURNING id", nName, nPass, nEmail);
    return r.empty() ? Account() : Account(number(r[0][0]), nName, nEmail, std::string(), true);
}

bool Account::validEmail(const std::string &nEmail){
    return pcrecpp::RE("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?", pcrecpp::RE_Options().set_caseless(true)).FullMatch(nEmail);
}

bool Account::checkPassword(const std::string &pw){
    return DB::query("SELECT EXISTS (SELECT 1 FROM users WHERE id = " + number(_id) +
                     " AND password = crypt($1, password))", pw)[0][0] == "t";
}

std::vector<Account> Account::select(const std::string &q, const std::string &param){
    std::string sql = "SELECT id, name, email, about FROM users " + q;
    DB::Result r = param.empty() ? DB::query(sql) : DB::query(sql, param);
    std::vector<Account> users(r.size());
    for(unsigned i=0; i<r.size(); i++)
        users[i] = Account(number(r[i][0]), r[i][1], r[i][2], r[i][3], false);
    return users;
}

std::vector<Account> Account::listArtists(unsigned int n, unsigned int begin){
    return select("WHERE EXISTS ( SELECT 1 FROM tracks WHERE user_id = users.id AND visible = 't' ) "
        "ORDER BY lower(name) ASC LIMIT "+number(n)+" OFFSET "+number(begin));
}

std::vector<Account> Account::search(const std::string &q){
    if(q.empty()) return std::vector<Account>();
    return select("WHERE name ILIKE $1 ORDER BY registration DESC", "%"+q+"%");
}
