#ifndef HTML_FORMS_H
#define HTML_FORMS_H

#include <string>

namespace Render {
namespace Html {

void searchForm(const std::string &action, const std::string &q=std::string());
void uploadForm(const std::string &action);

}
}

#endif // HTML_FORMS_H
