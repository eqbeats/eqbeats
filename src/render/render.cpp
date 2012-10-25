#include "render.h"
#include <iostream>
#include <cstdlib>

using namespace Render;

FCgiIO Render::o;

FCgiIO::FCgiIO()
  : std::ostream(&fOutBuf),
    fErr(&fErrBuf)
{}


void FCgiIO::attach(FCGX_Request* request){
    fRequest = request;

    // Reset error bits if there was an i/o error during the previous request
    if(bad()) clear();

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
