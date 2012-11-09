#ifndef CGI_H
#define CGI_H

#include <cgicc/Cgicc.h>
#include "path.h"

extern cgicc::Cgicc cgi; // defined in main.cpp

#define eqbeatsUrl() ((getenv("EQBEATS_HTTPS")? "https://" : "http://") + cgi.getEnvironment().getServerName())
#define getPath() stripSlash(cgi.getEnvironment().getPathInfo())

#endif // CGI_H
