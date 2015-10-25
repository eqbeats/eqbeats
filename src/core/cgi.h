#ifndef CGI_H
#define CGI_H

#include <cgicc/Cgicc.h>
#include "path.h"

extern cgicc::Cgicc cgi; // defined in cgi.cpp
extern char** headers;   // yep

#include <text/text.h>

#define eqbeatsUrl() ((getenv("EQBEATS_HTTPS")? "https://" : "http://") + cgi.getEnvironment().getServerName() + (cgi.getEnvironment().getServerPort() != 80 && (!getenv("EQBEATS_HTTPS") || cgi.getEnvironment().getServerPort() != 443)?(":" + number(cgi.getEnvironment().getServerPort())) : ""))

#endif // CGI_H
