#ifndef HTML_PAGE_H
#define HTML_PAGE_H
#include <string>

namespace Render {
namespace Html {

void header(const std::string &title=std::string(), const std::string &head=std::string(), int status=200);
void footer();

std::string metaDescription(std::string descr);

void notFound(const std::string &what="Page");
void errorPage(const std::string &err);

}
}

#endif // HTML_PAGE_H
