#ifndef REPL_H
#define REPL_H

#include <string>

class Repl{

    public:
        Repl() { s = 0; }
        Repl(const char *sock);
        ~Repl(){}

        operator bool() { return s != 0; }
        Repl& operator=(const Repl &r);

        std::string exec(std::string cmd);

    private:
        int s;

};

#endif // REPL_H
