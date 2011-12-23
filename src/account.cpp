#include <regex.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "account.h"
#include "db.h"
#include "utils.h"

Account::Account(int id): User(id){
    _email = getEmail();
    // (sends two requests)
}

Account::Account(const User &user): User(user){
    _email = getEmail();
}

Account::Account(int nId, const std::string &nName, const std::string &nEmail)
    : User(nId, nName){
    _email = nEmail;
}

std::string Account::getEmail() {
    unsigned int uid = htonl(id());
    const char* val[1] = { (char*) &uid };
    int len[1] = { sizeof(id()) };
    int bin[1] = { 1 };
    PGresult* result = PQexecParams(db, 
        "SELECT email FROM users WHERE id = $1",
        1, 0, val, len, bin, 0);
    std::string email = "";
    if(PQntuples(result) != 0)
        email = PQgetvalue(result, 0, 0);

    PQclear(result);
    return email;
}

bool Account::setPassword(const std::string &nPass){
    if(nPass.empty() || nPass.size()>72) return false;
    const char* val[2] = { number(id()).c_str(), nPass.c_str() };
    PQclear(PQexecParams(db,
        "UPDATE users SET password = crypt($2, gen_salt('bf')) WHERE id = $1",
        2, 0, val, 0, 0, 0));
    return true;
}

bool Account::setName(const std::string &nName){
    if(nName == name()) return true;
    if(nName.empty() || nName.size()>200) return false;
    if(exists(nName)) return false;
    _name = nName;
    return true;
}

bool Account::setEmail(const std::string &nEmail){
    if(nEmail == _email) return true;
    //if(!validEmail(nEmail)) return false;
    if(exists(std::string(), nEmail)) return false;
    _email = nEmail;
    return true;
}

void Account::commit(){
    const char* val[] = { number(id()).c_str(), name().c_str(), _email.c_str() };
    PQclear(PQexecParams(db,
        "UPDATE users SET name = $2, email = lower($3) WHERE id = $1",
        3, 0, val, 0, 0, 0));
}

bool Account::exists(const std::string &sName, const std::string &sEmail){
    const char* val[] = { sName.c_str(), sEmail.c_str() };
    PGresult* result = PQexecParams(db, "SELECT EXISTS ( SELECT id FROM users WHERE name = $1 OR email = lower($2) )",
        2, 0, val, 0, 0, 0);
    bool e = PQgetvalue(result, 0, 0)[0] == 't';
    PQclear(result);
    return e;
}

Account Account::create(const std::string &nName, const std::string &nPass, const std::string &nEmail){
    if(exists(nName, nEmail))
        return Account();

    const char* val[3] = { nName.c_str(), nPass.c_str(), nEmail.c_str() };
    PGresult* result = PQexecParams(db,
        "INSERT INTO users (name, password, email, registration, last_login) VALUES ($1, crypt($2, gen_salt('bf')), lower($3), 'now', 'now') "
        "RETURNING id",
        3, 0, val, 0, 0, 0);

    if(PQntuples(result) != 1){
        PQclear(result);
        return Account();
    }
    int id = atoi(PQgetvalue(result, 0, 0));

    PQclear(result);
    return Account(id, nName, nEmail);
}

bool Account::validEmail(const std::string &nEmail){
    regex_t regex;
    static const char *ex = "^[a-zA-Z0-9_.\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z0-9]+$";
    regcomp(&regex, ex, REG_EXTENDED|REG_ICASE|REG_NOSUB);
    int retValue = regexec(&regex, nEmail.c_str(), (size_t) 0, 0, 0);
    regfree(&regex);
    return retValue == 0;
}

bool Account::checkPassword(const std::string &pw){
    const char *val[] = { number(id()).c_str(), pw.c_str() };
    PGresult* result = PQexecParams(db,
        "SELECT EXISTS (SELECT 1 FROM users WHERE id = $1 AND password = crypt($2, password))"
        , 2, 0, val, 0, 0, 0);
    bool e = PQgetvalue(result, 0, 0)[0] == 't';
    PQclear(result);
    return e;
}
