#ifndef HTML_COMMENT_H
#define HTML_COMMENT_H

#include <string>
#include <vector>

class Comment;

namespace Render{
namespace Html{

void comments(const std::vector<Comment> &cmts, const std::string title="Comments");
void commentForm(const std::string &action);

}
}

#endif // HTML_COMMENT_H
