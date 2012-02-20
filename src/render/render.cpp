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
    fOutBuf.attach(request->out);
    fErrBuf.attach(request->err);

    // Parse environment
    for(char **e = fRequest->envp; *e != NULL; ++e) {
        std::string s(*e);
        std::string::size_type i = s.find('=');
        if(i == std::string::npos) return; // Illegally formed environment
        fEnv[s.substr(0, i)] = s.substr(i + 1);
    }
}
