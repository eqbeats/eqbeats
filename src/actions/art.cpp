#include "../session.h"
#include "../track.h"
#include "../user.h"
#include "../utils.h"
#include "actions.h"
#include "../html/html.h"

std::string Action::uploadArt(int tid, cgicc::Cgicc &cgi){
    cgicc::file_iterator file = cgi.getFile("file");
    if(Track(tid).artistId() == Session::user().id() && file != cgi.getFiles().end()){
        std::string filename = eqbeatsDir() + "/art/" + number(tid);
        std::ofstream out(filename.c_str(), std::ios_base::binary);
        file->writeToStream(out);
        out.close();
    }
    return Html::redirect(Track::url(tid));
}
