#ifndef CGI_H
#define CGI_H

#include <cgicc/Cgicc.h>
#include "path.h"

extern cgicc::Cgicc cgi; // defined in main.cpp
extern std::string path; // same

#define eqbeatsUrl() ((getenv("EQBEATS_HTTPS")? "https://" : "http://") + cgi.getEnvironment().getServerName())

#endif // CGI_H
