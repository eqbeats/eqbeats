#include "feed.h"
#include "../render.h"

using namespace Render;

std::string Html::atomFeed(const std::string &url){
    return "<link href=\"" + url + "\" type=\"application/atom+xml\" rel=\"alternate\" />";
}

void Html::feedIcon(const std::string &url){
    o << "<a class=\"feed\" title=\"Feed\" href=\"" + url + "\"><span>(Feed)</span></a>";
}
