#include "pages.h"
#include <core/cgi.h>

void Pages::statics(Document *doc){
    if(path == "/faq")
        doc->setHtml("html/faq.tpl", "FAQ");
    else if(path == "/credits")
        doc->moved("/thanks");
    else if(path == "/thanks")
        doc->setHtml("html/thanks.tpl", "Thanks");
    else if(path == "/api")
        doc->setHtml("html/api.tpl", "API");
}

