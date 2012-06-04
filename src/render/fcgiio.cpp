#include "fcgiio.h"
#include <iostream>
#include <cstdlib>

FCgiIO::FCgiIO()
  : std::ostream(&fOutBuf),
    fErr(&fErrBuf)
{}

void FCgiIO::attach(FCGX_Request* request){
    fRequest = request;
    fOutBuf.attach(request->out);
    fErrBuf.attach(request->err);

    // Parse environment
    fEnv.clear();
    for(char **e = fRequest->envp; *e != NULL; ++e) {
        std::string s(*e);
        std::string::size_type i = s.find('=');
        if(i == std::string::npos) return; // Illegally formed environment
        fEnv[s.substr(0, i)] = s.substr(i + 1);
    }
}
