#ifndef HTML_EVENT_H
#define HTML_EVENT_H

#include <vector>
#include "../../event.h"

namespace Render {
namespace Html {

void event(const Event &e);
void eventStream(const std::vector<Event> &events);

}
}

#endif //HTML_EVENT_H
