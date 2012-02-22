#include "forms.h"
#include "escape.h"
#include "../render.h"

using namespace Render;
using namespace std;

void Html::uploadForm(const std::string &action){
    o << "<div id=\"track-uploader\">"
            "<form action=\"" << action << "?js=no\" method=\"post\" enctype=\"multipart/form-data\" id=\"track-uploader-form\">"
                "<input type=\"file\" accept=\"audio/mpeg\" name=\"qqfile\" />"
                "<input type=\"submit\" value=\"Upload an MP3\" />"
            "</form>"
        "</div>"
        "<script src=\"/static/fileuploader.js\" type=\"text/javascript\"></script>"
        "<script>"
            "function createUploader(){"
                "var uploader = new qq.FileUploader({"
                    "element: document.getElementById('track-uploader'),"
                    "allowedExtensions: ['mp3'],"
                    "action: '" << action << "'"
                "});"
            "}"
            "window.onload = createUploader;"
        "</script>";
}

void Html::searchForm(const string &action, const string &q){
    o << "<form action=\"" << action << "\">"
            "<input type=\"text\" name=\"q\" value=\"" << escape(q) << "\" />"
            "<input type=\"submit\" value=\"Search\" />"
        "</form>";
}
