#ifndef SESSION_H
#define SESSION_H

#include <render/template.h>
#include "user.h"

namespace Session{

    void start();
    void start(std::string sid);
    void destroy();

    // Logged user.
    User user();

    std::string nonce();
    void newNonce();

    // Login. Return the sid or an empty string.
    std::string login(const User&);
    std::string login(const std::string &email, const std::string &pw);

    // Return true if the login is successful.
    // The sid is set if the user wasn't already logged.
    // Return false and fill the dictionary otherwise.
    bool login(Dict *d, std::string *sid);

    bool logout();

    Dict* fill(Dict*);

}

#endif // SESSION_H
