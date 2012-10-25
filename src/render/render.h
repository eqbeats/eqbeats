#ifndef RENDER_H
#define RENDER_H

#include <ostream>
#include <string>
#include <map>

#include "fcgio.h"
#include "cgicc/CgiInput.h"

namespace Render {

class FCgiIO : public cgicc::CgiInput, public std::ostream {

    public:

        FCgiIO();
        virtual inline ~FCgiIO() {}
        void attach(FCGX_Request* request);

        virtual inline size_t read(char *data, size_t length) { return FCGX_GetStr(data, length, fRequest->in); }
        virtual inline std::string getenv(const char *varName) { return fEnv[varName]; }
        inline std::ostream& err(void) { return fErr; }

    protected:
        FCGX_Request* 			fRequest;
        fcgi_streambuf			fOutBuf;
        fcgi_streambuf			fErrBuf;
        std::ostream 			fErr;
        std::map<std::string, std::string> 	fEnv;

};

extern FCgiIO o;

}

#endif // RENDER_H
