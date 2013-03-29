#ifndef TRACK_PAGES_H
#define TRACK_PAGES_H

#include <render/document.h>

namespace Pages {
    void art(Document*);
    void trackMisc(Document*);
    void trackActions(Document*);
    void JSONTrackActions(Document*);
    void trackUpload(Document*);
    void tracks(Document*);
    void oembed(Document*);
    void license(Document*);
}

#endif // TRACK_PAGES_H
