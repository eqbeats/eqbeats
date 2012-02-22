#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "../track.h"
#include "../art.h"

namespace Render {
namespace Http {

void downloadTrack(int tid, Track::Format f);
void trackArt(int tid, Art::Size sz=Art::Full);

}
}

#endif // DOWNLOAD_H
