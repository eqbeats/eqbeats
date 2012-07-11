#include "document.h"
#include "http.h"
#include <core/cgi.h>
#include <misc/timer.h>

#include <unistd.h>

Document::Document(){
    _code = 200;
    _mime = "text/html";
    _rootDict = new Dict("eqbeats");
    _rootDict->SetGlobalValue("EQBEATS_URL", eqbeatsUrl());
    _rootDict->SetGlobalValue("PATH", path);
    _dict = _rootDict;
}

Document::~Document(){
    delete _rootDict;
}

void Document::redirect(const std::string &r){
    _redir = r;
}

void Document::download(const File &f){
    _redir.clear();
    dw = f;
}

void Document::setTemplate(const std::string &tpl, const std::string &mime, int code){
    _redir.clear();
    if(dw) dw = File();
    _code = code;
    _mime = mime;
    _tpl = tpl;
}

void Document::setJson(const std::string &tpl, int code){
    setTemplate(tpl, "application/json", code);
    _dict = _rootDict->AddIncludeDictionary("JSON");
}

void Document::setHtml(const std::string &tpl, const std::string &title, int code){
    setTemplate(tpl, "text/html", code);
    _rootDict->SetValueAndShowSection("TITLE", title, "HAS_TITLE");
    _dict = _rootDict->AddIncludeDictionary("BODY");
}

std::string Document::generate(){
    if(!_redir.empty())
        return _http + Http::redirect(_redir);

    else if(dw)
        return _http + Http::download(dw);

    std::string out;
    if(_tpl.empty())
        setHtml("html/404.tpl", "404 Not Found", 404);

    if(_mime == "text/html" && _rootDict != _dict){
        _dict->SetFilename(_tpl);
        _rootDict->SetValueAndShowSection("REDIRECT", path, "HAS_REDIRECT");
        _rootDict->SetFormattedValue("GENERATION_TIME", "%lu µS", usecs());
        _rootDict->SetFormattedValue("PID", "%d", getpid());
        cache.ExpandWithData("html/page.tpl", ctemplate::STRIP_BLANK_LINES, _rootDict, NULL, &out);
    }

    else if(_mime == "application/json" && _rootDict != _dict){
        if(!cgi("jsonp").empty()){
            _mime = "text/javascript";
            _rootDict->SetValueAndShowSection("FUNCTION", cgi("jsonp"), "JSONP");
        }
        _dict->SetFilename(_tpl);
        cache.ExpandWithData("json/jsonp.tpl", ctemplate::STRIP_WHITESPACE, _rootDict, NULL, &out);
    }

    else
        cache.ExpandWithData(_tpl, ctemplate::STRIP_BLANK_LINES, _rootDict, NULL, &out);

    return _http + Http::header(_mime, _code) + out;
}
