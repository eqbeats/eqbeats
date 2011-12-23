#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLDoctype.h>
#include "FCgiIO.h"

using namespace std;
using namespace cgicc;

int main(int, char**){

    FCGX_Request request;
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while(FCGX_Accept_r(&request) == 0){

        FCgiIO io(request);
        Cgicc cgi(&io);

        // Upload

        file_iterator file = cgi.getFile("file");
        if (file != cgi.getFiles().end()) {
            ofstream out(("uploads/" + file->getFilename()).c_str(), ios_base::binary);
            file->writeToStream(out);
        }

        // Response

        io << HTTPHTMLHeader();

        io << "<html>"
                "<body>"
                    "<form method='POST' enctype='multipart/form-data'>"
                        "<input type='file' name='file'/>"
                        "<input type='submit'/>"
                    "</form>"
                "</body>"
            "</html>" << endl;

        FCGX_Finish_r(&request);
    }

    return 0;
}
