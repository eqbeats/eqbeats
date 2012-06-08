#include "file.h"

#define IFEXT(e, mime) if(ext == e) return mime;

std::string File::mimetype() const{
    size_t extPos = _filename.rfind('.');
    std::string ext;
    if(extPos != std::string::npos)
        ext = _filename.substr(extPos);

    IFEXT(".mp3", "audio/mpeg")
    IFEXT(".ogg", "audio/ogg")
    IFEXT(".wav", "audio/wav")
    IFEXT(".aif", "audio/aiff")
    IFEXT(".aiff", "audio/aiff")
    IFEXT(".m4a", "audio/mp4")
    IFEXT(".aac", "audio/aac")
    IFEXT(".flac", "audio/x-flac")

    IFEXT(".png", "image/png")
    IFEXT(".gif", "image/gif")
    IFEXT(".jpg", "image/jpeg")
    IFEXT(".jpeg", "image/jpeg")

    return "application/octet-stream";
}
