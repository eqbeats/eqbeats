#ifndef RENDER_DOCUMENT_H
#define RENDER_DOCUMENT_H

#include <core/template.h>

class Document {

    public:

        Document();
        ~Document();

        Dict* rootDict(){ return _rootDict; }
        Dict* dict(){ return _dict; }

        void redirect(const std::string &r);
        void setTemplate(const std::string &tpl, const std::string &mime, int code=200);
        void setHtml(const std::string &tpl, const std::string &title="", int code=200);
        void setJson(const std::string &tpl, int code=200);

        std::string generate();

        operator bool() const { return !_redir.empty() || !_tpl.empty(); }

    private:
        std::string _tpl;
        std::string _mime;
        std::string _redir;
        int _code;
        Dict *_rootDict, *_dict;

};

#endif // RENDER_DOCUMENT_H
