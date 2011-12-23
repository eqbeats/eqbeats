#include "html.h"

std::string Html::redirect(const std::string &location){
    return
        "Status: 303 See Other\n"
        "Location: " + location + "\n\n";
}
